package main

import (
	"bufio"
	"bytes"
	"fmt"
	"net/http"
	"os/exec"
	"strconv"
)

func main() {
	http.HandleFunc("/image", func(w http.ResponseWriter, r *http.Request) {
		ctx := r.Context()

		format := r.URL.Query().Get("format")
		if format == "" {
			http.Error(w, "format parameter missing or invalid", 400)
			return
		}

		width, err := strconv.Atoi(r.URL.Query().Get("width"))
		if err != nil {
			http.Error(w, "width parameter missing or invalid", 400)
			return
		}

		height, err := strconv.Atoi(r.URL.Query().Get("height"))
		if err != nil {
			http.Error(w, "height parameter missing or invalid", 400)
			return
		}

		stderr := bytes.NewBuffer(nil)

		cmd := exec.CommandContext(ctx,
			"./mkthumb",
			"-f", format,
			"-w", strconv.Itoa(width),
			"-h", strconv.Itoa(height))
		cmd.Env = []string{} // clear environment
		cmd.Stdin = r.Body
		cmd.Stdout = bufio.NewWriter(w)
		cmd.Stderr = stderr

		err = cmd.Run()
		if err != nil {
			http.Error(w, fmt.Sprintf("%s: %s", err.Error(), stderr.String()), 500)
			return
		}
	})

	http.ListenAndServe(":8080", nil)
}
