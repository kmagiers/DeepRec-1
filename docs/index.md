<h1 align="center">
    DeepRec
</h1>

# 简介
稀疏模型，是指在模型结构中离散特征计算逻辑占比较高的一类深度学习模型的统称。离散特征通常表现为id、tag、文字、词组等算法不能直接处理的非数值化特征，其广泛应用于搜索、广告、推荐等高价值业务中。当下主流开源深度学习框架，对稀疏模型的支持不足。在稀疏功能的支持、训练性能存在着问题，制约了稀疏模型的探索和发展。 

DeepRec(PAI-TF) 支持了淘宝搜索、猜你喜欢、定向、直通车等核心业务，支撑着千亿特征、万亿样本超大规模的稀疏训练。积累了核心的稀疏场景的功能及性能优化。针对稀疏模型在分布式、图优化、算子、Runtime等方面进行了深度的性能优化，同时提供了稀疏场景下特有的动态弹性特征，动态弹性维度，多Hash Embedding，自适应EmbeddingVariable、增量模型导出及加载等一系列功能。

# 开始

```{toctree}
:maxdepth: 2

Compile-And-Install
```

# 功能

```{toctree}
:maxdepth: 2
:caption: 稀疏功能

Embedding-Variable
Feature-Eviction
Feature-Filter
Dynamic-dimension-Embedding-Variable
Adaptive-Embedding
Multi-Hash-Variable
```

```{toctree}
:maxdepth: 2
:caption: 分布式训练

GRPC++
StarServer
```

```{toctree}
:maxdepth: 2
:caption: 图优化

Auto-Micro-Batch
Fused-Embedding
Smart-Stage
```

```{toctree}
:maxdepth: 2
:caption: Runtime优化

TensorPoolAllocator
```

```{toctree}
:maxdepth: 2
:caption: 模型导出

Incremental-Checkpoint
```

```{toctree}
:maxdepth: 2
:caption: 优化器

AdamAsync-Optimizer
AdagradDecay-Optimizer
```

```{toctree}
:maxdepth: 2
:caption: 算子及硬件加速

NVIDIA-TF32
oneDNN
```

```{toctree}
:maxdepth: 2
:caption: 样本读取及Dataset

WorkQueue
KafkaDataset
```
