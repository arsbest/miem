#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server.h"


int main(int argc, char **argv) {
    int fdpub;
    FILE *fp, *fppriv;
    struct message msg; /*��������� ���������, ������� �������� �� �������������� ������������ ������*/

    while (1) {
        /* ������������� ����� ����������� ��� ������. ��������� ����� open() �����������, ���� ������� �������-������ �� ��������� ������ ����� ������*/
        if ((fdpub = open("public", O_RDONLY)) == -1) {
            perror("[Server] Opening public fifo");
            exit(1);
        }

        /* �� �������������� ������ �������� ���������, ���������� ����� ������� ������ � */
        /* ���������� �����. ��������� ���� ����������� ��� ������. */
        if (read(fdpub, (char*)&msg, sizeof(msg)) == -1) {
            perror("[Server] Reading message struct");
            exit(1);
        }

        if ((fp = fopen(msg.filename, "r")) == NULL) {
            perror("[Server] Opening file");
            exit(1);
        }

        /* ������ ����� ����������� ��� ������. �������� ������ ����, ���� ������ �� ������� ���� ����� ������� ������. ���� ��������
        ��������, � ������ �� ����� ������� �����, ������ �������� ������ ����� open(). !��. ��������������� �����, ��������������� ����������. */
        if ((fppriv = fopen(msg.privfifo, "w")) == NULL) {
            perror("[Server] Opening private fifo");
            exit(1);
        }

        /* ������ �� ����� ���������� � ������ ����������� ����� */
        int c, i = 0, row = 0, page = 0;
        while ((c = getc(fp)) != EOF) {
            i++;
            if (c == '\n') c = ' ';
            putc(c, fppriv);
            if (i == 50) {
                i = 0;
                putc('\n', fppriv);
                row++;
                if (row == 24) {
                    row = 0;
                    page++;
                    fprintf(fppriv, "<<<%d>>>\n\n", page);
                }
            }
        }

        if (i > 1) {
            page++;
            fprintf(fppriv, "\n<<<%d>>>\n", page);
        }

        /* ����� ����������� ���������, ��������� ���� � ������ ����������� ����� ����������� */
        fclose(fp);
        fclose(fppriv);

        /* ������� ������������� �����. ������ ��������� � ���������� ����� �����. ������� ��� �������� � ���������� �������� ������� � ���, ��� �������, ����� �������� ������ ���� �� open(), ������ ��������. */
        close(fdpub);
    }

    return 0;
}
