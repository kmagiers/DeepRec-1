# SparseOperationKit Release Notes #
The release notes for SparseOperationKit.

## What's new in Version 1.0.1 ##
+ Supports Horovod as the synchronized training communication tool.
+ Supports dynamic input in All2AllDenseEmbedding, which means `unique->lookup->gather` pattern can be used.
+ Supports IdentityHashtable, which means no hash-mapping during inserting new keys.
+ Added TF Distributed Embedding totally with TF's ops.

## What's new in Version 1.0.0 ##
+ Implemented a new framework that can be used to easily integrate different embedding algorithms to common DL frameworks.
+ Supports single-node & multi-node synchronized training with TensorFlow.
+ Integrated HugeCTR's DistributedSparseEmbedding algorithm.
+ Integrated All2AllDenseEmbedding algorithm.
+ Added custom Adam optimizer for SOK when TF version <= 2.4.