#copyright (c) 2022 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

ARG UBUNTU_VERSION

FROM ubuntu:${UBUNTU_VERSION}

ARG DEBIAN_FRONTEND=noninteractive

HEALTHCHECK NONE
RUN useradd -d /home/itex -m -s /bin/bash itex

RUN apt-get update && \
    apt-get install -y --no-install-recommends --fix-missing \
    apt-utils \
    build-essential \
    ca-certificates \
    clinfo \
    curl \
    git \
    gnupg2 \
    gpg-agent \
    rsync \
    sudo \
    unzip \
    wget && \
    apt-get clean && \
    rm -rf  /var/lib/apt/lists/*

RUN echo "intelpython=exclude" > $HOME/cfg.txt

ENV LANG=C.UTF-8

ARG PYTHON=python3.9

RUN apt-get update && apt-get install -y --no-install-recommends --fix-missing \
    ${PYTHON} lib${PYTHON} python3-pip && \
    apt-get clean && \
    rm -rf  /var/lib/apt/lists/*

RUN pip --no-cache-dir install --upgrade \
    pip \
    setuptools

RUN ln -sf $(which ${PYTHON}) /usr/local/bin/python && \
    ln -sf $(which ${PYTHON}) /usr/local/bin/python3 && \
    ln -sf $(which ${PYTHON}) /usr/bin/python && \
    ln -sf $(which ${PYTHON}) /usr/bin/python3

RUN  sed -i "/if util.abstract_sockets_supported:/d"  /usr/lib/python3.9/multiprocessing/connection.py && \
     sed -i  "/{next(_mmap_counter)}/d"  /usr/lib/python3.9/multiprocessing/connection.py

ARG TF_VER="2.12"

RUN pip --no-cache-dir install tensorflow==${TF_VER}

ARG TF_PLUGIN_WHEEL

COPY models/binaries/$TF_PLUGIN_WHEEL /tmp/itex_cpu_whls/

RUN pip install /tmp/itex_cpu_whls/* && \
    rm -rf /tmp/itex_cpu_whls

ENV DNNL_MAX_CPU_ISA="AVX512_CORE_AMX"
ENV TF_ENABLE_MKL_NATIVE_FORMAT=1

ADD https://raw.githubusercontent.com/intel/intel-extension-for-tensorflow/master/third-party-programs/dockerlayer/THIRD-PARTY-PROGRAMS.txt /licenses/
ADD https://raw.githubusercontent.com/intel/intel-extension-for-tensorflow/master/third-party-programs/dockerlayer/third-party-program-of-intel-extension-for-tensorflow.txt /licenses/
ADD https://raw.githubusercontent.com/intel/intel-extension-for-tensorflow/master/third-party-programs/dockerlayer/third-party-programs-of-intel-tensorflow.txt /licenses/