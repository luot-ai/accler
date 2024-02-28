#!/bin/bash

# 检查命令行参数是否指定了 Docker 容器名称，否则使用默认值
DOCKERNAME="${1:-craz_heyrovsky}"

# 在指定的 Docker 容器中执行交互式的 bash shell
docker exec -it "${DOCKERNAME}" bash

docker exec -it crazy_heyrovsky bash
