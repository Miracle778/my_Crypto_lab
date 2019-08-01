package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
)

var (
	message string
	cipher  string
	key     int
)

type Msg struct {
	Flag int // 选择功能
	Data string
	Key  int
}

func main() {
	msg, err := typeIn()
	for err != nil {
		fmt.Println(err, "\n--------------------")
		msg, err = typeIn()
	}
	out := caesar(msg)
	fmt.Println(out)
}

func typeIn() (*Msg, error) {
	In := new(Msg)
	inputerr := errors.New("输入错误,请重新输入")

	fmt.Println("请输入0或1选择功能:\n0.加密\n1.解密")
	fmt.Scanf("%d", &In.Flag)
	//这两行相当于c语言getchar
	inputReader := bufio.NewReader(os.Stdin)
	_, err := inputReader.ReadString('\n')

	if In.Flag != 0 && In.Flag != 1 {
		return In, inputerr
	}
	fmt.Println("输入你的明文或者密文")

	Data, _, err := inputReader.ReadLine()
	In.Data = string(Data)

	fmt.Println("输入密钥key")
	fmt.Scanf("%d", &In.Key)

	//吃回车
	_, err = inputReader.ReadString('\n')

	if err != nil {
		panic("typeIn()出错")
	}

	return In, nil
}

func caesar(msg *Msg) string {
	data := []byte(msg.Data)
	var keyS, keyN byte
	if msg.Flag == 0 {
		//加密
		keyS = byte(msg.Key % 26)
		keyN = byte(msg.Key % 10)
	} else {
		//解密
		keyS = 26 - byte(msg.Key%26)
		keyN = 10 - byte(msg.Key%10)
	}
	for k, v := range data {
		if v >= 97 && v <= 122 {
			//小写字母
			if v+keyS > 122 {
				v = v + keyS - 26
			} else {
				v = v + keyS
			}
			data[k] = v
		} else if v >= 65 && v <= 90 {
			//大写字母
			if v+keyS > 90 {
				v = v + keyS - 26
			} else {
				v = v + keyS
			}
			data[k] = v
		} else if v >= 48 && v <= 57 {
			//数字
			if v+keyN > 57 {
				v = v + keyN - 10
			} else {
				v = v + keyN
			}
			data[k] = v
		}
	}
	res := string(data)
	return res
}
