#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server.h"

int main(int argc, char **argv) {
    int fdpub, fdpriv, fd;
    struct message msg; /*��������� ���������, ������� �������� �� �������������� ������������ ������*/
    int n;
    char line[LINESIZE];

    while (1) {
        /* ������������� ����� ����������� ��� ������. ��������� ����� open() �����������, ���� ������� �������-������ �� ��������� ������ ����� ������*/
        if ((fdpriv = open(PUBLIC, O_RDONLY)) == -1) {
            perror(PUBLIC);
            exit(1);
        }

        /* �� �������������� ������ �������� ���������, ���������� ����� ������� ������ � */
        /* ���������� �����. ��������� ���� ����������� ��� ������. */
        while(read(fdpub, (char*)&msg, sizeof(msg)) > 0)
        {
            if ((fd = open(msg.filename, O_RDONLY)) == -1) {
                perror(msg.filename);
                break;
            }

            /* ������ ����� ����������� ��� ������. �������� ������ ����, ���� ������ �� ������� ���� ����� ������� ������. ���� ��������
            ��������, � ������ �� ����� ������� �����, ������ �������� ������ ����� open(). !��. ��������������� �����, ��������������� ����������. */
            if ((fdpriv = open(msg.privfifo, O_WRONLY)) == -1) {
                perror(msg.privfifo);
                break;
            }

            /* ������ �� ����� ���������� � ������ ����������� ����� */
            while((n = read(fd, line, LINESIZE)) > 0) {
                write(fdpriv, line, n);
            }
            /* ����� ����������� ���������, ��������� ���� � ������ ����������� ����� ����������� */
            close(fd);
            close(fdpriv);
        }
        /* end of first while() */
        /* ������� ������������� �����. ������ ��������� � ���������� ����� �����. ������� ��� �������� � ���������� �������� ������� � ���, ��� �������, ����� �������� ������ ���� �� open(), ������ ��������. */
        close(fdpub);
    }

    return 0;
}
