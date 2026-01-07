FROM nvcr.io/nvidia/deepstream:8.0-triton-multiarch

SHELL ["/bin/bash", "-c"]

RUN apt-get update && \
    apt-get install -y \
        vim \
        wget \
        zip \
        python3-pip \
        python3-venv \
        python3-gi \
        python3-dev \
        python3-gst-1.0 \
        python-gi-dev \
        meson \
        cmake \
        g++ \
        build-essential \
        libglib2.0-dev \
        libglib2.0-dev-bin \
        libgstreamer1.0-dev \
        libtool \
        m4 \
        autoconf \
        automake \
        libcairo2-dev \
        libgstrtspserver-1.0-0 \
        gstreamer1.0-rtsp \
        libgirepository1.0-dev \
        gobject-introspection \
        gir1.2-gst-rtsp-server-1.0 \
        ffmpeg \
        libopencv-dev \
        libflac-dev \
        libdca-dev \
        libdvdread-dev \ 
        libmp3lame-dev \
        libmpg123-dev \
        libmjpegutils-2.1-0t64 \
        libdvdnav-dev \
        sudo \
        git && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

RUN usermod -aG sudo triton-server && \
    echo "triton-server ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

RUN pip install \
        jupyterlab \
        jupyterhub \
        pycairo \
        requests \
        redis \
        opencv-python \
        ffmpeg-python \
        cuda-python==12.8.0 \
        ipyleaflet==0.17.4 \
        ipywidgets==8.0.7 \
        jupyterlab-widgets 

# Install pyds from NVIDIA AI IOT GitHub releases
ARG PYDS_VER=1.2.2
RUN python3 -m pip install --no-cache-dir \
  "https://github.com/NVIDIA-AI-IOT/deepstream_python_apps/releases/download/v${PYDS_VER}/pyds-${PYDS_VER}-cp312-cp312-linux_x86_64.whl"

RUN /opt/nvidia/deepstream/deepstream-8.0/user_additional_install.sh

RUN git clone https://github.com/NVIDIA-AI-IOT/deepstream_python_apps.git \
    /opt/nvidia/deepstream/deepstream/samples/deepstream_python_apps

RUN chown -R 1000:1000 /opt/nvidia/deepstream/deepstream-8.0

# CMD ["jupyter", "lab", "--ip=0.0.0.0", "--port=8888", "--no-browser", "--allow-root", "--ServerApp.token=", "--ServerApp.password="]
CMD ["jupyter","lab", \
     "--ip=0.0.0.0","--port=8888", \
     "--no-browser","--allow-root", \
     "--ServerApp.token=","--ServerApp.password=", \
     "--ServerApp.trust_xheaders=True", \
     "--ServerApp.allow_remote_access=True", \
     "--ServerApp.allow_origin=*", \
     "--ServerApp.disable_check_xsrf=True"]
