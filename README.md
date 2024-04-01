# MIPS Assembler + Emulator

## MIPS Assembler

## 목표
<p align="center">
  <img src="https://github.com/JunHyungJang/MIPS_Assembler/assets/89409079/27b698d8-278e-4921-9631-6599ee8c03a9" align="center" width="32%">  
  <img src="https://github.com/JunHyungJang/MIPS_Assembler/assets/89409079/001b7625-2d9b-4ee3-bbf4-cc6364cf8301" align="center" width="32%">  
  <figcaption align="center">
</figcaption></p>

왼쪽과 같은 어셈블리 코드를 오른쪽과 같은 바이너리 코드로 변환하는것을 목표로 한다 


## 과정

![image](https://github.com/JunHyungJang/MIPS_Assembler/assets/89409079/9d61a36e-60c7-441a-a53d-890ad5d2c0a4)

MIPS Assembler에서 지원해야 하는 명령어들은 위와 같이 정의되어있다. 

![image](https://github.com/JunHyungJang/MIPS_Assembler/assets/89409079/beb3c8fb-2810-49cd-8d2b-96f35261e229)

3가지 종류의 Instruction 종류에 맞게 변환하는 작업을 거친다. 


## 해결방법

fopen함수를 통해서 어셈블리 파일을 열었고, 한 줄을 읽는 것은 fgets 함수를 사용하였고
strtok() 함수를 사용해서 공백을 기준으로 단어를 받았습니다. 입력받은 단어가 .data 혹은 .text 이라면, 그 후로는 각각 data, text section이므로 한줄을 입력받 을때마다 cnt 를 증가시켜줬습니다.
Data를 받을때는 data_section이라는 구조체를 활용하여 data를 읽고 적절한 구조체 변수에 값
들을 할당시켜줬습니다.

한 줄을 읽을 때 마다 pc값을 +4씩 증가시켜주었고, data section에서 text section으로 바뀔때는
메모리 hierarchy 구조에 맞게 0x400000으로 바꾼 후에 +4씩 증가시켜 줬습니다. 만약 입력받은
line이 공백이거나 label만 들어있는 경우에는 메모리를 차지하지 않기 때문에 구조체에 이 pc
address를 할당하지 않았고 pc값고 증가시키지 않았습니다. Text를 받을때는 instr_one 이라는 구조체를 활용하여 text를 읽고 적절한 구조체 변수에 값을 할당시켜줬습니다.

Data, text section이 끝날 때 finish라는 단어를 구조체에 할당시켜줘서, 파일쓰기 과정에서 for
loop를 돌 때 finish를 만나면 실행종료하게 만들었습니다.
파일을 읽는것과 동시에 o파일에 함수들을 쓰지 않았고, 한번 파일을 다 읽고 다시 구조체들을
for loop를 통해 돌리고 o파일에 작성하였습니다. o파일의 가장 위에는 text, data section의number 가장 마지막에는 value 값들을 출력해야 하는데, 이러한 경우에 순차적으로 o파일에 작
성시 순서가 꼬이기 때문입니다.

