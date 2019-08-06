package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"strconv"
	"time"
)

var pc1Table = []int{
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4} //密钥处理PC-1置换

var lsTable = []int{
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1} //移位表

var pc2Table = []int{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32} //密钥处理PC-2置换

var ipTable = []int{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7} //IP置换

var eTable = []int{
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1} //圈函数扩展变换表

var pTable = []int{
	16, 7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2, 8, 24, 14,
	32, 27, 3, 9,
	19, 13, 30, 6,
	22, 11, 4, 25} //圈函数P盒置换

var sTable = []int{
	//S1第一行10书上是0待检验
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 15, 10, 0, 6, 13,
	//S2
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
	//S3
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
	//S4
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
	//S5
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
	//S6
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
	//S7
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
	//S8
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11} //圈函数S盒

var ipnTable = []int{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25} //IP逆置换

func flagInit(d *bool, obj, data, key, file, outfilename *string) {
	flag.BoolVar(d, "d", false, "选择解密功能，默认flase")
	flag.StringVar(obj, "obj", "", "选择对象，默认标准输入，可选file")
	flag.StringVar(data, "data", "", "密文或明文")
	flag.StringVar(key, "key", "", "密钥")
	flag.StringVar(file, "file", "", "如果对象是文件的话，用file输入文件名")
	flag.StringVar(outfilename, "outfile", "", "输出文件名")
	flag.Parse()
	flag.PrintDefaults()
	if len(*key) != 8 {
		log.Fatal("密钥长度必须为8字节")
	}

}

func main() {
	var (
		d           bool
		obj         string
		data        string
		key         string
		file        string
		outfilename string
	)
	flagInit(&d, &obj, &data, &key, &file, &outfilename)
	subkey := generateKey(key)
	if obj == "file" {
		desFile(file, d, subkey, outfilename)
	} else if obj == "" {
		des(data, d, subkey)
	}
}

func desFile(filename string, decodeFlag bool, subkey map[int][]int, outname string) {
	start := time.Now() //计时
	f, err := os.Open(filename)
	fw, _ := os.Create(outname)
	if err != nil {
		log.Fatal(err)
	}
	defer func() {
		if err = f.Close(); err != nil {
			log.Fatal(err)
		}
	}()
	r := bufio.NewReader(f)
	b := make([]byte, 8) //每次读8字节
	for i := 0; ; i++ {
		_, err := r.Read(b)
		if err != nil {
			// fmt.Println("出错", err)
			break
		}
		temp1 := des(string(b), decodeFlag, subkey)
		temp2 := []byte(bitTostr(temp1))
		fw.Write(temp2)
	}
	fmt.Println("加/解密文件共用时:", time.Since(start))
}

func generateKey(key string) map[int][]int { //返回密钥
	res := make(map[int][]int)
	dataBin := byteTobin(key)
	key56 := tansform(dataBin, pc1Table, 56) //PC-1置换
	left, right := key56[:28], key56[28:]    //C0 D0
	for i := 0; i < 16; i++ {
		left = append(left[lsTable[i]:], left[:lsTable[i]]...)
		right = append(right[lsTable[i]:], left[:lsTable[i]]...)
		temp := append(left, right...)
		res[i] = tansform(temp, pc2Table, 48)
	}
	return res
}

func byteTobin(data string) []int {
	dataByte := []byte(data)
	res := make([]int, 64) //DES分组为64比特
	tempStr := ""
	for _, v := range dataByte {
		temp := strconv.FormatUint(uint64(v), 2) //二进制字符串
		for len(temp) < 8 {                      //填充
			temp = "0" + temp
		}
		tempStr += temp
	}
	for i := 0; i < len(tempStr); i++ {
		res[i], _ = strconv.Atoi(string(tempStr[i]))
	}
	return res
}

func bitTostr(data []int) string {
	num := len(data) / 8
	dataStr := ""
	res := make([]byte, num)
	for i := 0; i < len(data); i++ {
		dataStr += strconv.Itoa(data[i]) //int转为二进制串
	}
	for i := 0; i < num; i++ {
		temp := dataStr[8*i : 8*(i+1)]
		t, _ := strconv.ParseUint(temp, 2, 8)
		res[i] = byte(t)
	}
	// fmt.Println(res)
	// fmt.Println(string(res), ":", len(string(res)))
	return string(res)
}
func tansform(in []int, table []int, len int) []int {
	res := make([]int, len)
	for i := 0; i < len; i++ {
		res[i] = in[table[i]-1] //这里注意减1
	}
	return res
}

func des(data string, decodeFlag bool, subkey map[int][]int) []int { //flag选择加密还是解密，flase加密，true解密
	subKey := subkey
	if decodeFlag == true {
		tempMap := make(map[int][]int)
		for i := 0; i < 16; i++ {
			tempMap[i] = subkey[15-i]
		}
		subKey = tempMap
	}
	dataLen := len(data)
	if dataLen%8 != 0 { //明文需要填充
		var t byte //填充byte 0
		for i := 0; i < (8 - (dataLen % 8)); i++ {
			data = data + string(t)
		}
	}
	num := len(data) / 8 //分组组数
	var res []int
	for i := 0; i < num; i++ {
		dataBit := byteTobin(data[8*i : 8*(i+1)])
		dataBit = tansform(dataBit, ipTable, 64)
		left, right := dataBit[:32], dataBit[32:]
		for j := 0; j < 16; j++ {
			temp := left //暂时保存下left
			left = right //L(i) = R(i-1)
			right = xor(temp, fFunc(right, subKey[j]))
		}
		temp := append(right, left...)
		temp = tansform(temp, ipnTable, 64)
		res = append(res, temp...)
	}
	return res
}
func xor(a, b []int) []int {
	if len(a) != len(b) {
		log.Fatal("长度不等，异或出错")
	}
	res := make([]int, len(a))
	for i := 0; i < len(a); i++ {
		res[i] = a[i] ^ b[i]
	}
	return res
}
func fFunc(right []int, subkey []int) []int {
	res := tansform(right, eTable, 48)
	res = xor(res, subkey)
	res = sBox(res)                 //S-盒替代
	res = tansform(res, pTable, 32) //P-盒置换
	return res
}

func sBox(d []int) []int {
	resStr := ""
	for i := 0; i < 8; i++ {
		s := d[6*i : 6*(i+1)]
		x := s[0]*2 + s[5]
		y := s[1]*8 + s[2]*4 + s[3]*2 + s[4]
		// fmt.Println("sbox", x*16+y+64*i)
		temp := int64(sTable[x*16+y+64*i])
		res := strconv.FormatInt(temp, 2) //转2进制字符串
		for len(res) < 4 {
			res = "0" + res //填充
		}
		resStr += res
	}
	if len(resStr) != 32 {
		log.Fatal("S盒置换出错")
	}
	resInt := make([]int, 32)
	for i := 0; i < 32; i++ {
		resInt[i], _ = strconv.Atoi(string(resStr[i]))
	}
	return resInt
}
