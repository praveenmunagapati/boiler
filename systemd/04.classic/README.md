Starting a classic server at boot, causing dependents to wait

This example shows systemd starting our example service upon reaching the
multi-user target. The service creates the socket itself. This is classic
behavior, not using a systemd unit to create the socket.

The downside to this approach, is that all dependent services would have to be
set up to start after this one. If we use systemd socket activation instead,
systemd would create the socket and start us and our dependents all at once.

The changes are:

 * remove the example.socket unit since we make the socket ourselves
 * change srv.c (the code for the example server) to bind the socket

Installation:

    make
    sudo cp srv /usr/bin/example
    sudo cp example.service /etc/systemd/system
    sudo systemctl enable example.service
    sudo systemctl start example.service

If you reboot now, the example process comes up automatically.

Shut down and clean up:

    sudo systemctl stop example.service
    sudo systemctl disable example.service
    sudo rm -f /etc/systemd/system/example.service
    sudo rm -f /usr/bin/example
