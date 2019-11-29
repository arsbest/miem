#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "server.h"

int main(int argc, char **argv) {
    int n, fdpub, fdpriv, fd;
    struct message msg; /*��������� ���������, ������� �������� �� �������������� ������������ ������*/
    char line[LINESIZE];

    while (1) {
        /* ������������� ����� ����������� ��� ������. ��������� ����� open() �����������, ���� ������� �������-������ �� ��������� ������ ����� ������*/
        if ((fdpub = open(PUBLIC, O_RDONLY)) == -1) {
            perror(PUBLIC);
            exit(1);
        }

        /* �� �������������� ������ �������� ���������, ���������� ����� ������� ������ � */
        /* ���������� �����. ��������� ���� ����������� ��� ������. */
        if (read(fdpub, (char*)&msg, sizeof(msg)) == -1) {
            perror("Reading msg from client");
            exit(1);
        }

        if ((fd = open(msg.filename, O_RDONLY)) == -1) {
            perror(msg.filename);
            exit(1);
        }

        /* ������ ����� ����������� ��� ������. �������� ������ ����, ���� ������ �� ������� ���� ����� ������� ������. ���� ��������
        ��������, � ������ �� ����� ������� �����, ������ �������� ������ ����� open(). !��. ��������������� �����, ��������������� ����������. */
        if ((fdpriv = open(msg.privfifo, O_WRONLY)) == -1) {
            perror(msg.privfifo);
            exit(1);
        }

        /* ������ �� ����� ���������� � ������ ����������� ����� */
        while((n = read(fd, line, LINESIZE)) > 0) {
            write(fdpriv, line, n);
        }

        /* ����� ����������� ���������, ��������� ���� � ������ ����������� ����� ����������� */
        close(fd);
        close(fdpriv);

        /* ������� ������������� �����. ������ ��������� � ���������� ����� �����. ������� ��� �������� � ���������� �������� ������� � ���, ��� �������, ����� �������� ������ ���� �� open(), ������ ��������. */
        close(fdpub);
    }

    return 0;
}
