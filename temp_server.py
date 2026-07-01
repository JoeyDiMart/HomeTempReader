#!/usr/bin/env python3
"""
Minimal local HTTP service that runs the compiled DS18B20 reader (./main)
on every request and returns the current temperature in Fahrenheit.

This binds to localhost only. Exposure to the outside world is handled
separately by a reverse SSH tunnel to the AWS instance, not by this script.
"""

import http.server
import subprocess
import os

HOST = "127.0.0.1"
PORT = 5000

# Absolute path to the compiled binary, so this works regardless of the
# working directory the service happens to be started from (e.g. systemd).
BINARY_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "main")


class TemperatureHandler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            result = subprocess.run(
                [BINARY_PATH],
                capture_output=True,
                text=True,
                timeout=5,
            )
        except Exception as e:
            self.send_error(500, f"Failed to run sensor program: {e}")
            return

        if result.returncode != 0:
            # main returns non-zero on a bad CRC or read failure --
            # surface that instead of pretending we have a valid number.
            self.send_error(503, f"Sensor read failed: {result.stderr.strip()}")
            return

        temp_f = result.stdout.strip()

        body = f"{temp_f}\n".encode("utf-8")
        self.send_response(200)
        self.send_header("Content-Type", "text/plain")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def log_message(self, format, *args):
        # Default logging goes to stderr, which systemd/journald captures.
        super().log_message(format, *args)


if __name__ == "__main__":
    server = http.server.HTTPServer((HOST, PORT), TemperatureHandler)
    print(f"Serving live temperature readings on http://{HOST}:{PORT}")
    server.serve_forever()