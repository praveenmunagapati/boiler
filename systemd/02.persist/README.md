Simple on-demand persistent service

Compared to 01.ondemand, this version of the service is still started
on-demand (when a client initially connects), but the difference is that
it persists instead of exiting after serving the client.

This shows how systemd will work with either a one-shot or a persistent
service. For a server that persists, systemd knows that since the process
is still running, it does not need to spawn another process for each client
connection.

    make
    sudo cp srv /usr/bin/example
    sudo cp example.socket /etc/systemd/system
    sudo cp example.service /etc/systemd/system
    sudo systemctl enable example.socket
    sudo systemctl start example.socket

Connect:

    ./cli -f /tmp/example.sock

Verify the /usr/bin/example serviec has persisted, noting the PID:

    ps aux | grep example

You can verify the PID of the service remains the same after servicing further
connections.

    ./cli -f /tmp/example.sock
    ps aux | grep example

Shut down and clean up:

    sudo systemctl stop example.socket 
    sudo systemctl disable example.socket 
    sudo rm -f /etc/systemd/system/example.socket 
    sudo rm -f /etc/systemd/system/example.service
    sudo rm -f /usr/bin/example
