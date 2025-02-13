# Common Guide for Running

## Prepare

### Intel GPU Driver

Refer to [Install Intel GPU driver](./../docs/install/install_for_gpu.md#install-gpu-drivers)

### Intel® oneAPI Base Toolkit

Refer to [Install Intel® oneAPI Base Toolkit](./../docs/install/install_for_gpu.md#install-oneapi-base-toolkit-packages)

### Setup Running Environment
1. Python

    We recommend you use Python 3.9 or newer.  Check which version you're running by using this command:
    ```
    python -V
    ```

2. Create Virtual Environment

    
    We recommend you create a virtual Python environment called env_itex and install official Tensorflow* in it, like this:
    
    ```
    export ENV_NAME=env_itex
    rm -rf $ENV_NAME
    python -m venv $ENV_NAME
    source $ENV_NAME/bin/activate
    pip install --upgrade pip
    pip install tensorflow
    ```

3. Install Intel® Extension for TensorFlow*

    ```
    pip install --upgrade intel-extension-for-tensorflow[gpu]
    ```

## Running
### Enable oneAPI Running Environment

```
source /opt/intel/oneapi/setvars.sh
```

### Enable Virtual Running Environment

```
source env_itex/bin/activate
```

### Run Script

Run your script with Intel® Extension for TensorFlow*.
