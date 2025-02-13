# Quick Example on Intel CPU and GPU

## Installation

* If you are using a heterogeneous machine that contains the Intel GPU, please refer to [Intel GPU Software Installation](../docs/install/install_for_gpu.md)

* Otherwise, please refer to [Intel CPU Software Installation](../docs/install/experimental/install_for_cpu.md)

## Code

Use TensorFlow to compute graph: Conv -> ReLU activation -> Bias

### quick_example.py
```python
import numpy as np
import sys

import tensorflow as tf

# Conv + ReLU activation + Bias
N = 1
num_channel = 3
input_width, input_height = (5, 5)
filter_width, filter_height = (2, 2)

x = np.random.rand(N, input_width, input_height, num_channel).astype(np.float32)
weight = np.random.rand(filter_width, filter_height, num_channel, num_channel).astype(np.float32)
bias = np.random.rand(num_channel).astype(np.float32)

conv = tf.nn.conv2d(x, weight, strides=[1, 1, 1, 1], padding='SAME')
activation = tf.nn.relu(conv)
result = tf.nn.bias_add(activation, bias)

print(result)
print('Finished')
```

* Execute the Code
```
python quick_example.py
```

## Example Output
With successful execution, it will print out the following results:
```
...
tf.Tensor(
[[[[3.479142   2.7296917  4.6456823 ]
   [4.077278   3.9259825  5.3000765 ]
   [3.3999124  3.0527704  4.0656753 ]
   [2.85485    2.7297122  3.9373732 ]
   [2.4818356  2.1455178  2.4929404 ]]

  [[3.6422923  2.718459   4.7090344 ]
   [3.988714   3.3391027  4.875052  ]
   [3.6461415  2.9349675  4.327398  ]
   [3.298973   2.3905785  4.1704025 ]
   [1.9154005  1.6926193  1.9677248 ]]

  [[3.481086   2.9746864  3.8941312 ]
   [3.3221133  2.5479512  4.197306  ]
   [3.305706   2.9873173  4.5597944 ]
   [3.250221   3.118212   3.8672705 ]
   [1.949225   1.2636094  1.5300783 ]]

  [[3.1403804  2.1729176  3.6628485 ]
   [3.2607155  2.6342418  3.9381838 ]
   [2.6761076  2.5063303  3.4718971 ]
   [2.8880196  2.1658201  3.3787665 ]
   [2.1193419  1.42261    2.318963  ]]

  [[1.8809638  1.6514435  2.3549364 ]
   [1.8598063  1.517385   1.9702091 ]
   [1.9260886  1.3804817  2.381424  ]
   [1.6027272  1.7787259  1.9631021 ]
   [0.93901324 1.2134862  0.89942324]]]], shape=(1, 5, 5, 3), dtype=float32)
Finished
```

## Notes
1. In this example, it is not necessary to import intel_extension_for_tensorflow, and no need to call any of its APIs.<br/>
If installed as the `intel-extension-for-tensorflow[cpu]`, then the script will choose CPU as the backend and be executed on the CPU automatically; while if installed as `intel-extension-for-tensorflow[gpu]`, then the default backend will be GPU and the script will be executed on the GPU.
