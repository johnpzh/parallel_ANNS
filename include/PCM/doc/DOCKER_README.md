--------------------------------------------------------------------------------
How To Run Processor Counter Monitor Server Container from GitHub Container Repository or Docker Hub
--------------------------------------------------------------------------------

As root user:
1. ``modprobe msr``
2. ``docker run -d --name pcm --privileged -p 9738:9738 ghcr.io/opcm/pcm`` (GitHub Container repository) or ``docker run -d --name pcm --privileged -p 9738:9738 opcm/pcm`` (Dockerhub repository)
   - the container can also be run with limited capabilities without the privileged mode: ``docker run -d --name pcm --cap-add=SYS_ADMIN --cap-add=SYS_RAWIO --device=/dev/cpu --device=/dev/mem -v /sys/firmware/acpi/tables/MCFG:/pcm/sys/firmware/acpi/tables/MCFG:ro -v /proc/bus/pci/:/pcm/proc/bus/pci/ -v /proc/sys/kernel/nmi_watchdog:/pcm/proc/sys/kernel/nmi_watchdog -p 9738:9738 ghcr.io/opcm/pcm`` (there is also a docker-compose file containing these options: https://raw.githubusercontent.com/opcm/pcm/master/docker-compose.yml)

This will start pcm-sensor-server container exposing CPU metrics from the whole system at port 9738 

The URLs of the docker container repositories:
- https://github.com/opcm/pcm/pkgs/container/pcm
- https://hub.docker.com/r/opcm/pcm
