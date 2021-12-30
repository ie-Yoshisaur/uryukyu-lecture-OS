package fileWrite

import "testing"

func TestFileWrite(t *testing.T) {
    result := Hello("Yoshisaur")
    want := "Hi, Yoshisaur. Welcome!"
    if result != want {
        t.Errorf("fileWrite.Hello() = %q want %q", result, want)
    }
}
