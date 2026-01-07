docker build -t deepstream-8-jupiter -f Dockerfile .
docker run --gpus all -d --rm \
  --network=host --ipc=host \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /home/n03an/code/deepstream_custom_parser:/opt/nvidia/deepstream/deepstream/sources/deepstream_custom_parser \
  -e DISPLAY=$DISPLAY \
  -w /opt/nvidia/deepstream/deepstream-8.0 \
  deepstream-8-jupyter