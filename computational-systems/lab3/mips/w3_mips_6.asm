.data
one: .float 1
a: .float 9909.23
S: .float 0
str: .asciiz "S = "

.text
main:
lwc1 $f1, one # ��������� � $f1 �������

lwc1 $f0, a # ��������� � $f0 ���� �������� ����� A
sqrt.s $f0, $f0 # ��������� sqrt(A)
add.s $f0, $f0, $f1 # ��������� 1
swc1 $f0, S # ��������� � ���������� S ��� ���������

# ������� "S = "
li $v0, 4
la $a0, str
syscall

# ������� �����
lwc1 $f12, S
li $v0, 2
syscall
