package main

import (
	"fmt"
	"os"
	"time"
)

func main() {
	f, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println(err.Error())
		return
	}
	fmt.Printf("dmesg -c; rmmod print_block_device; insmod print_block_device.ko pid=%d fd=%d \n", os.Getpid(), f.Fd())
	for {
		time.Sleep(time.Second * 100)
	}
	f.Close()
}
