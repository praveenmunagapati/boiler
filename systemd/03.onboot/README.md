Starting process at boot instead of on-demand

This example shows starting of our dummy service "example" at boot, or
more accurately, when systemd at boot up reaches the multi-user target.
We still let systemd make the socket for us because, that way, systemd
can start us and our dependents all at once.

The change is simply to add this line to the Install section of the .service file:

    WantedBy=multi-user.target

Installation:

    make
    sudo cp srv /usr/bin/example
    sudo cp example.socket /etc/systemd/system
    sudo cp example.service /etc/systemd/system
    sudo systemctl enable example.socket
    sudo systemctl start example.socket
    sudo systemctl enable example.service
    sudo systemctl start example.service

The service is now running. If you reboot now, then when the system has
finished booting, notice that the example service is already running.
This is what makes it distinct from on-demand startup.

    ps aux | grep example

Shut down and clean up:

    sudo systemctl stop example.service
    sudo systemctl disable example.service
    sudo systemctl stop example.socket
    sudo systemctl disable example.socket
    sudo rm -f /etc/systemd/system/example.service
    sudo rm -f /etc/systemd/system/example.socket
    sudo rm -f /usr/bin/example
