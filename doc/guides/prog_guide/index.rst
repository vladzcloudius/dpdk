..  BSD LICENSE
    Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.
    * Neither the name of Intel Corporation nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Programmer's Guide
==================

|today|


INFORMATION IN THIS DOCUMENT IS PROVIDED IN CONNECTION WITH INTEL PRODUCTS. NO LICENSE, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE,
TO ANY INTELLECTUAL PROPERTY RIGHTS IS GRANTED BY THIS DOCUMENT. EXCEPT AS PROVIDED IN INTEL'S TERMS AND CONDITIONS OF SALE FOR SUCH PRODUCTS,
INTEL ASSUMES NO LIABILITY WHATSOEVER AND INTEL DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY, RELATING TO SALE AND/OR USE OF INTEL PRODUCTS INCLUDING LIABILITY
OR WARRANTIES RELATING TO FITNESS FOR A PARTICULAR PURPOSE, MERCHANTABILITY, OR INFRINGEMENT OF ANY PATENT, COPYRIGHT OR OTHER INTELLECTUAL PROPERTY RIGHT.

A "Mission Critical Application" is any application in which failure of the Intel Product could result, directly or indirectly, in personal injury or death.
SHOULD YOU PURCHASE OR USE INTEL'S PRODUCTS FOR ANY SUCH MISSION CRITICAL APPLICATION, YOU SHALL INDEMNIFY AND HOLD INTEL AND ITS SUBSIDIARIES, SUBCONTRACTORS AND AFFILIATES,
AND THE DIRECTORS, OFFICERS, AND EMPLOYEES OF EACH, HARMLESS AGAINST ALL CLAIMS COSTS, DAMAGES, AND EXPENSES AND REASONABLE ATTORNEYS' FEES ARISING OUT OF, DIRECTLY OR INDIRECTLY,
ANY CLAIM OF PRODUCT LIABILITY, PERSONAL INJURY, OR DEATH ARISING IN ANY WAY OUT OF SUCH MISSION CRITICAL APPLICATION,
WHETHER OR NOT INTEL OR ITS SUBCONTRACTOR WAS NEGLIGENT IN THE DESIGN, MANUFACTURE, OR WARNING OF THE INTEL PRODUCT OR ANY OF ITS PARTS.

Intel may make changes to specifications and product descriptions at any time, without notice.
Designers must not rely on the absence or characteristics of any features or instructions marked "reserved" or "undefined".
Intel reserves these for future definition and shall have no responsibility whatsoever for conflicts or incompatibilities arising from future changes to them.
The information here is subject to change without notice. Do not finalize a design with this information.

The products described in this document may contain design defects or errors known as errata which may cause the product to deviate from published specifications.
Current characterized errata are available on request.

Contact your local Intel sales office or your distributor to obtain the latest specifications and before placing your product order.

Copies of documents which have an order number and are referenced in this document, or other Intel literature, may be obtained by calling 1-800-548- 4725,
or go to: http://www.intel.com/design/literature.htm.

Any software source code reprinted in this document is furnished for informational purposes only and may only be used or copied and no license, express or implied,
by estoppel or otherwise, to any of the reprinted source code is granted by this document.

Code Names are only for use by Intel to identify products, platforms, programs, services, etc.
("products") in development by Intel that have not been made commercially available to the public, i.e., announced, launched or shipped.
They are never to be used as "commercial" names for products. Also, they are not intended to function as trademarks.

Intel and the Intel logo are trademarks of Intel Corporation in the U.S. and/or other countries.

\*Other names and brands may be claimed as the property of others.

Copyright © 2012-2014, Intel Corporation. All rights reserved.

**Contents**

.. toctree::
    :maxdepth: 3
    :numbered:

    intro
    overview
    env_abstraction_layer
    malloc_lib
    ring_lib
    mempool_lib
    mbuf_lib
    poll_mode_drv
    i40e_ixgbe_igb_virt_func_drv
    driver_vm_emul_dev
    ivshmem_lib
    poll_mode_drv_emulated_virtio_nic
    poll_mode_drv_paravirtual_vmxnets_nic
    intel_dpdk_xen_based_packet_switch_sol
    libpcap_ring_based_poll_mode_drv
    link_bonding_poll_mode_drv_lib
    timer_lib
    hash_lib
    lpm_lib
    lpm6_lib
    packet_distrib_lib
    ip_fragment_reassembly_lib
    multi_proc_support
    kernel_nic_interface
    thread_safety_intel_dpdk_functions
    qos_framework
    power_man
    packet_classif_access_ctrl
    packet_framework
    vhost_lib
    source_org
    dev_kit_build_system
    dev_kit_root_make_help
    extend_intel_dpdk
    build_app
    ext_app_lib_make_help
    perf_opt_guidelines
    writing_efficient_code
    profile_app
    glossary


**Figures**

:ref:`Figure 1. Core Components Architecture <pg_figure_1>`

:ref:`Figure 2. EAL Initialization in a Linux Application Environment <pg_figure_2>`

:ref:`Figure 3. Example of a malloc heap and malloc elements within the malloc library <pg_figure_3>`

:ref:`Figure 4. Ring Structure <pg_figure_4>`

:ref:`Figure 5. Two Channels and Quad-ranked DIMM Example <pg_figure_5>`

:ref:`Figure 6. Three Channels and Two Dual-ranked DIMM Example <pg_figure_6>`

:ref:`Figure 7. A mempool in Memory with its Associated Ring <pg_figure_7>`

:ref:`Figure 8. An mbuf with One Segment <pg_figure_8>`

:ref:`Figure 9. An mbuf with Three Segments <pg_figure_9>`

:ref:`Figure 10. Virtualization for a Single Port NIC in SR-IOV Mode <pg_figure_10>`

:ref:`Figure 11. Performance Benchmark Setup <pg_figure_11>`

:ref:`Figure 12. Fast Host-based Packet Processing <pg_figure_12>`

:ref:`Figure 13. Inter-VM Communication <pg_figure_13>`

:ref:`Figure 14. Host2VM Communication Example Using kni vhost Back End <pg_figure_14>`

:ref:`Figure 15. Host2VM Communication Example Using qemu vhost Back End <pg_figure_15>`

:ref:`Figure 16. Memory Sharing inthe Intel® DPDK Multi-process Sample Application <pg_figure_16>`

:ref:`Figure 17. Components of an Intel® DPDK KNI Application <pg_figure_17>`

:ref:`Figure 18. Packet Flow via mbufs in the Intel DPDK® KNI <pg_figure_18>`

:ref:`Figure 19. vHost-net Architecture Overview <pg_figure_19>`

:ref:`Figure 20. KNI Traffic Flow <pg_figure_20>`

:ref:`Figure 21. Complex Packet Processing Pipeline with QoS Support <pg_figure_21>`

:ref:`Figure 22. Hierarchical Scheduler Block Internal Diagram <pg_figure_22>`

:ref:`Figure 23. Scheduling Hierarchy per Port <pg_figure_23>`

:ref:`Figure 24. Internal Data Structures per Port <pg_figure_24>`

:ref:`Figure 25. Prefetch Pipeline for the Hierarchical Scheduler Enqueue Operation <pg_figure_25>`

:ref:`Figure 26. Pipe Prefetch State Machine for the Hierarchical Scheduler Dequeue Operation <pg_figure_26>`

:ref:`Figure 27. High-level Block Diagram of the Intel® DPDK Dropper <pg_figure_27>`

:ref:`Figure 28. Flow Through the Dropper <pg_figure_28>`

:ref:`Figure 29. Example Data Flow Through Dropper <pg_figure_29>`

:ref:`Figure 30. Packet Drop Probability for a Given RED Configuration <pg_figure_30>`

:ref:`Figure 31. Initial Drop Probability (pb), Actual Drop probability (pa) Computed Using a Factor 1 (Blue Curve) and a Factor 2 (Red Curve) <pg_figure_31>`

:ref:`Figure 32. Example of packet processing pipeline. The input ports 0 and 1 are connected with the output ports 0, 1 and 2 through tables 0 and 1. <pg_figure_32>`

:ref:`Figure 33. Sequence of steps for hash table operations in packet processing context <pg_figure_33>`

:ref:`Figure 34. Data structures for configurable key size hash tables <pg_figure_34>`

:ref:`Figure 35. Bucket search pipeline for key lookup operation (configurable key size hash tables) <pg_figure_35>`

:ref:`Figure 36. Pseudo-code for match, match_many and match_pos <pg_figure_36>`

:ref:`Figure 37. Data structures for 8-byte key hash tables <pg_figure_37>`

:ref:`Figure 38. Data structures for 16-byte key hash tables <pg_figure_38>`

:ref:`Figure 39. Bucket search pipeline for key lookup operation (single key size hash tables) <pg_figure_39>`

**Tables**

:ref:`Table 1. Packet Processing Pipeline Implementing QoS <pg_table_1>`

:ref:`Table 2. Infrastructure Blocks Used by the Packet Processing Pipeline <pg_table_2>`

:ref:`Table 3. Port Scheduling Hierarchy <pg_table_3>`

:ref:`Table 4. Scheduler Internal Data Structures per Port <pg_table_4>`

:ref:`Table 5. Ethernet Frame Overhead Fields <pg_table_5>`

:ref:`Table 6. Token Bucket Generic Operations <pg_table_6>`

:ref:`Table 7. Token Bucket Generic Parameters <pg_table_7>`

:ref:`Table 8. Token Bucket Persistent Data Structure <pg_table_8>`

:ref:`Table 9. Token Bucket Operations <pg_table_9>`

:ref:`Table 10. Subport/Pipe Traffic Class Upper Limit Enforcement Persistent Data Structure <pg_table_10>`

:ref:`Table 11. Subport/Pipe Traffic Class Upper Limit Enforcement Operations <pg_table_11>`

:ref:`Table 12. Weighted Round Robin (WRR) <pg_table_12>`

:ref:`Table 13. Subport Traffic Class Oversubscription <pg_table_13>`

:ref:`Table 14. Watermark Propagation from Subport Level to Member Pipes at the Beginning of Each Traffic Class Upper Limit Enforcement Period <pg_table_14>`

:ref:`Table 15. Watermark Calculation <pg_table_15>`

:ref:`Table 16. RED Configuration Parameters <pg_table_16>`

:ref:`Table 17. Relative Performance of Alternative Approaches <pg_table_17>`

:ref:`Table 18. RED Configuration Corresponding to RED Configuration File <pg_table_18>`

:ref:`Table 19. Port types <pg_table_19>`

:ref:`Table 20. Port abstract interface <pg_table_20>`

:ref:`Table 21. Table types <pg_table_21>`

:ref:`Table 29. Table Abstract Interface <pg_table_29_1>`

:ref:`Table 22. Configuration parameters common for all hash table types <pg_table_22>`

:ref:`Table 23. Configuration parameters specific to extendible bucket hash table <pg_table_23>`

:ref:`Table 24. Configuration parameters specific to pre-computed key signature hash table <pg_table_24>`

:ref:`Table 25. The main large data structures (arrays) used for configurable key size hash tables <pg_table_25>`

:ref:`Table 26. Field description for bucket array entry (configurable key size hash tables) <pg_table_26>`

:ref:`Table 27. Description of the bucket search pipeline stages (configurable key size hash tables) <pg_table_27>`

:ref:`Table 28. Lookup tables for match, match_many, match_pos <pg_table_28>`

:ref:`Table 29. Collapsed lookup tables for match, match_many and match_pos <pg_table_29>`

:ref:`Table 30. The main large data structures (arrays) used for 8-byte and 16-byte key size hash tables <pg_table_30>`

:ref:`Table 31. Field description for bucket array entry (8-byte and 16-byte key hash tables) <pg_table_31>`

:ref:`Table 32. Description of the bucket search pipeline stages (8-byte and 16-byte key hash tables) <pg_table_32>`

:ref:`Table 33. Next hop actions (reserved) <pg_table_33>`

:ref:`Table 34. User action examples <pg_table_34>`
