package main

import (
	"os"
	"github.com/go-git/go-git/v5"
	. "github.com/go-git/go-git/v5/_examples"
)


func main() {
    // Clone the given repository to the given directory
    Info("git clone https://github.com/Vegz78/McAirpos")

    _, err := git.PlainClone("/home/pi/McAirpos", false, &git.CloneOptions{
        URL:      "https://github.com/Vegz78/McAirpos.git",
        Progress: os.Stdout,
    })

    CheckIfError(err)
}