# Build and run:
#   docker build -t carina_dev_container_img .
#   docker run -d --cap-add sys_ptrace -p 2222:22 -p 3390:3390 --name carina_dev_container
#
# stop:
#   docker stop carina_dev_container
#
# ssh credentials:
#   user: root
#   password: password

FROM ubuntu:latest
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update && apt-get -y upgrade

# Install and configure xrdp
RUN apt-get install -y xrdp
RUN cp /etc/xrdp/xrdp.ini /etc/xrdp/xrdp.ini.bak && \
    sed -i 's/3389/3390/g' /etc/xrdp/xrdp.ini && \
    sed -i 's/max_bpp=32/#max_bpp=32\nmax_bpp=128/g' /etc/xrdp/xrdp.ini && \
    sed -i 's/xserverbpp=24/#xserverbpp=24\nxserverbpp=128/g' /etc/xrdp/xrdp.ini
# Workaround to start xfce session - remove last two lines of this file, then add 'startxfce4'
RUN sed -i "$ d" /etc/xrdp/startwm.sh && sed -i "$ d" /etc/xrdp/startwm.sh && echo 'startxfce4' >> /etc/xrdp/startwm.sh

# Change root password to allow login
RUN echo 'root:password' | chpasswd

# Install xfce and alternate terminal (the default one doesn't always work)
RUN apt-get install -y xfce4 && apt-get install -y xfce4-terminal && echo "2" | update-alternatives --config x-terminal-emulator

# Install Qt
RUN apt-get install -y build-essential qtcreator qt5-default

# Other stuff
RUN apt-get install -y ssh git gdb cmake

# Set up ssh
RUN ( \
    echo 'LogLevel DEBUG2'; \
    echo 'PermitRootLogin yes'; \
    echo 'PasswordAuthentication yes'; \
    echo 'Subsystem sftp /usr/lib/openssh/sftp-server'; \
  ) > /etc/ssh/sshd_config_test_clion \
  && mkdir /run/sshd

# Start ssh server
CMD ["/usr/sbin/sshd", "-D", "-e", "-f", "/etc/ssh/sshd_config_test_clion"]