Simple on-demand persistent service

The change from 01.example to 02.persist is only in the srv.c logic.
Instead of exiting, the server stays around to accept more connections.
This shows how systemd will work with either a one-shot or a persistent
service. For a server that persists, systemd knows that since the process
is still running, it does not need to spawn another process for each client
connection.

    % make
    % sudo cp srv /usr/bin/example
    % sudo cp example.socket /etc/systemd/system
    % sudo cp example.service /etc/systemd/system
    % sudo systemctl daemon-reload
    % sudo systemctl start example.socket

Connect:

    %  ./cli -f /tmp/example.sock

Note that process persists since it didn't exit. You can reconnect to it. Note
the pid remains the same.

    % ps aux | grep example
    %  ./cli -f /tmp/example.sock
    % ps aux | grep example

Shut it down:

    % sudo systemctl stop example.socket example.service
