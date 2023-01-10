package fileWrite

import (
    "io"
    "os"
    "time"
    "fmt"
    "bufio"
)

func WriteSingleByte(writer io.Writer) {
    singleByteData := []byte("0")
    writer.Write(singleByteData)
}

func TimeWriteWithoutBuffer(fileByteSize int) int64 {
    filePath := "experiment/txt/file.txt"
    writer, err := os.Create(filePath)
    if err != nil {
        fmt.Println(err)
    }
    defer writer.Close()
    start := time.Now()
    for i := 0; i < fileByteSize; i++ {
        WriteSingleByte(writer)
    }
    executionTime := time.Since(start).Nanoseconds()
    return executionTime
}

func TimeWriteWithBuffer(fileByteSize int, bufferByteSize int) int64 {
    filePath := "experiment/txt/file.txt"
    writer, err := os.Create(filePath)
    if err != nil {
        fmt.Println(err)
    }
    defer writer.Close()
    bufferedWriter := bufio.NewWriterSize(writer, bufferByteSize)
    defer bufferedWriter.Flush()
    start := time.Now()
    for i := 0; i < fileByteSize; i++ {
        WriteSingleByte(bufferedWriter)
    }
    executionTime := time.Since(start).Nanoseconds()
    return executionTime
}
