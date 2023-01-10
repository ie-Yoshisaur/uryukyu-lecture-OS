package main

import (
    "encoding/csv"
    "fmt"
    "os"
    "gitlab.ie.u-ryukyu.ac.jp/os/2022/e205723-fileWrite/fileWrite"
)

const (
    singleKiloBytes = 1024
)

var (
    fileSizeLimit1 = 8192
    bufferSizeArray1 = []int{1, 2, 4, 8, 16, 32, 64, 128, 256, 512}
    fileSizeLimit2 = 16384
    bufferSizeArray2 = []int{512, 1024, 2048, 4096, 8192, 16384}
)

func generateTimeResultRecords(fileSizeLimit int, bufferSizeArray []int) [][]string {
    // following 4 lines make [][]string slice
    timeResultRecords := make([][]string, fileSizeLimit)
    for i_fileSize := range timeResultRecords {
        timeResultRecords[i_fileSize] = make([]string, len(bufferSizeArray) + 1)
    }

    for i_fileSize := 0; i_fileSize * singleKiloBytes / 8 < fileSizeLimit; i_fileSize++ {
        fileSize := (i_fileSize + 1) * singleKiloBytes / 8
        timeResultRecords[i_fileSize][0] = fmt.Sprintf("%d", fileSize)
        for i_bufferSize, bufferSize := range bufferSizeArray {
            if bufferSize == 1 {
                // if bufferSize is 1 Byte, it can be considered writing without buffer
                timeResultRecords[i_fileSize][i_bufferSize + 1] = fmt.Sprintf("%d", fileWrite.TimeWriteWithoutBuffer(fileSize))
            } else {
                // if bufferSize is not 1 Byte, it can be considered writing with buffer
                timeResultRecords[i_fileSize][i_bufferSize + 1] = fmt.Sprintf("%d", fileWrite.TimeWriteWithBuffer(fileSize, bufferSize))
            }
        }
    }
    return timeResultRecords
}

func saveTimeRecordAsCsv(csvFilePath string, timeResultRecords [][]string) {
    // the result of experiment is saved in the file "time_results.csv"
    csvFile, _ := os.Create(csvFilePath)
    defer csvFile.Close()
    csvWriter := csv.NewWriter(csvFile)
    defer csvWriter.Flush()
    for _, record := range timeResultRecords {
        csvWriter.Write(record)
    }
}

func main() {
    timeResultRecords1 := generateTimeResultRecords(fileSizeLimit1, bufferSizeArray1)
    csvFilePath1 := "./experiment/csv/time_results1.csv"
    saveTimeRecordAsCsv(csvFilePath1, timeResultRecords1)

    timeResultRecords2 := generateTimeResultRecords(fileSizeLimit2, bufferSizeArray2)
    csvFilePath2 := "./experiment/csv/time_results2.csv"
    saveTimeRecordAsCsv(csvFilePath2, timeResultRecords2)
}
