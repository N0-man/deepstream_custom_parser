docker buildx build --load -t n0man/nvidia-deepstream:8.0-triton-multiarch-jupyter -f Dockerfile .

docker run --gpus all -d --rm \
  --network=host --ipc=host \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v /home/n03an/code/deepstream_custom_parser:/opt/nvidia/deepstream/deepstream/sources \
  -e DISPLAY=$DISPLAY \
  -w /opt/nvidia/deepstream/deepstream-8.0 \
  n0man/nvidia-deepstream:8.0-triton-multiarch-jupyter