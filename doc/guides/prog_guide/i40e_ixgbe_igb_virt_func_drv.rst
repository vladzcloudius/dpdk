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

I40E/IXGBE/IGB Virtual Function Driver
======================================

Supported Intel® Ethernet Controllers (see the *Intel® DPDK Release Notes* for details)
support the following modes of operation in a virtualized environment:

*   **SR-IOV mode**: Involves direct assignment of part of the port resources to different guest operating systems
    using the PCI-SIG Single Root I/O Virtualization (SR IOV) standard,
    also known as "native mode" or"pass-through" mode.
    In this chapter, this mode is referred to as IOV mode.

*   **VMDq mode**: Involves central management of the networking resources by an IO Virtual Machine (IOVM) or
    a Virtual Machine Monitor (VMM), also known as software switch acceleration mode.
    In this chapter, this mode is referred to as the Next Generation VMDq mode.

SR-IOV Mode Utilization in an Intel® DPDK Environment
-----------------------------------------------------

The Intel® DPDK uses the SR-IOV feature for hardware-based I/O sharing in IOV mode.
Therefore, it is possible to partition SR-IOV capability on Ethernet controller NIC resources logically and
expose them to a virtual machine as a separate PCI function called a "Virtual Function".
Refer to Figure 10.

Therefore, a NIC is logically distributed among multiple virtual machines (as shown in Figure 10),
while still having global data in common to share with the Physical Function and other Virtual Functions.
The Intel® DPDK i40evf, igbvf or ixgbevf as a Poll Mode Driver (PMD) serves for the Intel® 82576 Gigabit Ethernet Controller,
Intel® Ethernet Controller I350 family, Intel® 82599 10 Gigabit Ethernet Controller NIC,
or Intel® Fortville 10/40 Gigabit Ethernet Controller NIC's virtual PCI function.
Meanwhile the Intel® DPDK Poll Mode Driver (PMD) also supports "Physical Function" of such NIC's on the host.

The Intel® DPDK PF/VF Poll Mode Driver (PMD) supports the Layer 2 switch on Intel® 82576 Gigabit Ethernet Controller,
Intel® Ethernet Controller I350 family, Intel® 82599 10 Gigabit Ethernet Controller,
and Intel® Fortville 10/40 Gigabit Ethernet Controller NICs so that guest can choose it for inter virtual machine traffic in SR-IOV mode.

For more detail on SR-IOV, please refer to the following documents:

*   `SR-IOV provides hardware based I/O sharing <http://www.intel.com/network/connectivity/solutions/vmdc.htm>`_

*   `PCI-SIG-Single Root I/O Virtualization Support on IA
    <http://www.intel.com/content/www/us/en/pci-express/pci-sig-single-root-io-virtualization-support-in-virtualization-technology-for-connectivity-paper.html>`_

*   `Scalable I/O Virtualized Servers <http://www.intel.com/content/www/us/en/virtualization/server-virtualization/scalable-i-o-virtualized-servers-paper.html>`_

.. _pg_figure_10:

**Figure 10. Virtualization for a Single Port NIC in SR-IOV Mode**

.. image24_png has been renamed

|single_port_nic|

Physical and Virtual Function Infrastructure
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following describes the Physical Function and Virtual Functions infrastructure for the supported Ethernet Controller NICs.

Virtual Functions operate under the respective Physical Function on the same NIC Port and therefore have no access
to the global NIC resources that are shared between other functions for the same NIC port.

A Virtual Function has basic access to the queue resources and control structures of the queues assigned to it.
For global resource access, a Virtual Function has to send a request to the Physical Function for that port,
and the Physical Function operates on the global resources on behalf of the Virtual Function.
For this out-of-band communication, an SR-IOV enabled NIC provides a memory buffer for each Virtual Function,
which is called a "Mailbox".

Intel® Fortville 10/40 Gigabit Ethernet Controller VF Infrastructure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In a virtualized environment, the programmer can enable a maximum of *128 Virtual Functions (VF)*
globally per Intel® Fortville 10/40 Gigabit Ethernet Controller NIC device.
Each VF can have a maximum of 16 queue pairs.
The Physical Function in host could be either configured by the Linux* i40e driver
(in the case of the Linux Kernel-based Virtual Machine [KVM]) or by DPDK PMD PF driver.
When using both DPDK PMD PF/VF drivers, the whole NIC will be taken over by DPDK based application.

For example,

*   Using Linux* i40e  driver:

    .. code-block:: console

        rmmod i40e (To remove the i40e module)
        insmod i40e.ko max_vfs=2,2 (To enable two Virtual Functions per port)

*   Using the Intel® DPDK PMD PF i40e driver:

    Kernel Params: iommu=pt, intel_iommu=on

    .. code-block:: console

        modprobe uio
        insmod igb_uio
        ./dpdk_nic_bind.py -b igb_uio bb:ss.f
        echo 2 > /sys/bus/pci/devices/0000\:bb\:ss.f/max_vfs (To enable two VFs on a specific PCI device)

    Launch the Intel® DPDK testpmd/example or your own host daemon application using the Intel® DPDK PMD library.

Virtual Function enumeration is performed in the following sequence by the Linux* pci driver for a dual-port NIC.
When you enable the four Virtual Functions with the above command, the four enabled functions have a Function#
represented by (Bus#, Device#, Function#) in sequence starting from 0 to 3.
However:

*   Virtual Functions 0 and 2 belong to Physical Function 0

*   Virtual Functions 1 and 3 belong to Physical Function 1

.. note::

    The above is an important consideration to take into account when targeting specific packets to a selected port.

Intel® 82599 10 Gigabit Ethernet Controller VF Infrastructure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The programmer can enable a maximum of *63 Virtual Functions* and there must be *one Physical Function* per Intel® 82599
10 Gigabit Ethernet Controller NIC port.
The reason for this is that the device allows for a maximum of 128 queues per port and a virtual/physical function has to
have at least one queue pair (RX/TX).
The current implementation of the Intel® DPDK ixgbevf driver supports a single queue pair (RX/TX) per Virtual Function.
The Physical Function in host could be either configured by the Linux* ixgbe driver
(in the case of the Linux Kernel-based Virtual Machine [KVM]) or by DPDK PMD PF driver.
When using both DPDK PMD PF/VF drivers, the whole NIC will be taken over by DPDK based application.

For example,

*   Using Linux* ixgbe driver:

    .. code-block:: console

        rmmod ixgbe (To remove the ixgbe module)
        insmod ixgbe max_vfs=2,2 (To enable two Virtual Functions per port)

*   Using the Intel® DPDK PMD PF ixgbe driver:

    Kernel Params: iommu=pt, intel_iommu=on

    .. code-block:: console

        modprobe uio
        insmod igb_uio
        ./dpdk_nic_bind.py -b igb_uio bb:ss.f
        echo 2 > /sys/bus/pci/devices/0000\:bb\:ss.f/max_vfs (To enable two VFs on a specific PCI device)

    Launch the Intel® DPDK testpmd/example or your own host daemon application using the Intel® DPDK PMD library.

Virtual Function enumeration is performed in the following sequence by the Linux* pci driver for a dual-port NIC.
When you enable the four Virtual Functions with the above command, the four enabled functions have a Function#
represented by (Bus#, Device#, Function#) in sequence starting from 0 to 3.
However:

*   Virtual Functions 0 and 2 belong to Physical Function 0

*   Virtual Functions 1 and 3 belong to Physical Function 1

.. note::

    The above is an important consideration to take into account when targeting specific packets to a selected port.

Intel® 82576 Gigabit Ethernet Controller and Intel® Ethernet Controller I350 Family VF Infrastructure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In a virtualized environment, an Intel® 82576 Gigabit Ethernet Controller serves up to eight virtual machines (VMs).
The controller has 16 TX and 16 RX queues.
They are generally referred to (or thought of) as queue pairs (one TX and one RX queue).
This gives the controller 16 queue pairs.

A pool is a group of queue pairs for assignment to the same VF, used for transmit and receive operations.
The controller has eight pools, with each pool containing two queue pairs, that is, two TX and two RX queues assigned to each VF.

In a virtualized environment, an Intel® Ethernet Controller I350 family device serves up to eight virtual machines (VMs) per port.
The eight queues can be accessed by eight different VMs if configured correctly (the i350 has 4x1GbE ports each with 8T X and 8 RX queues),
that means, one Transmit and one Receive queue assigned to each VF.

For example,

*   Using Linux* igb driver:

    .. code-block:: console

        rmmod igb (To remove the igb module)
        insmod igb max_vfs=2,2 (To enable two Virtual Functions per port)

*   Using Intel®  DPDK PMD PF igb driver:

    Kernel Params: iommu=pt, intel_iommu=on modprobe uio

    .. code-block:: console

        insmod igb_uio
        ./dpdk_nic_bind.py -b igb_uio bb:ss.f
        echo 2 > /sys/bus/pci/devices/0000\:bb\:ss.f/max_vfs (To enable two VFs on a specific pci device)

    Launch Intel® DPDK testpmd/example or your own host daemon application using the Intel® DPDK PMD library.

Virtual Function enumeration is performed in the following sequence by the Linux* pci driver for a four-port NIC.
When you enable the four Virtual Functions with the above command, the four enabled functions have a Function#
represented by (Bus#, Device#, Function#) in sequence, starting from 0 to 7.
However:

*   Virtual Functions 0 and 4 belong to Physical Function 0

*   Virtual Functions 1 and 5 belong to Physical Function 1

*   Virtual Functions 2 and 6 belong to Physical Function 2

*   Virtual Functions 3 and 7 belong to Physical Function 3

.. note::

    The above is an important consideration to take into account when targeting specific packets to a selected port.

Validated Hypervisors
~~~~~~~~~~~~~~~~~~~~~

The validated hypervisor is:

*   KVM (Kernel Virtual Machine) with  Qemu, version 0.14.0

However, the hypervisor is bypassed to configure the Virtual Function devices using the Mailbox interface,
the solution is hypervisor-agnostic.
Xen* and VMware* (when SR- IOV is supported) will also be able to support the Intel® DPDK with Virtual Function driver support.

Expected Guest Operating System in Virtual Machine
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The expected guest operating systems in a virtualized environment are:

*   Fedora* 14 (64-bit)

*   Ubuntu* 10.04 (64-bit)

For supported kernel versions, refer to the *Intel® DPDK Release Notes*.

Setting Up a KVM Virtual Machine Monitor
----------------------------------------

The following describes a target environment:

*   Host Operating System: Fedora 14

*   Hypervisor: KVM (Kernel Virtual Machine) with Qemu  version 0.14.0

*   Guest Operating System: Fedora 14

*   Linux Kernel Version: Refer to the  *Intel® DPDK Getting Started Guide*

*   Target Applications:  l2fwd, l3fwd-vf

The setup procedure is as follows:

#.  Before booting the Host OS, open **BIOS setup** and enable **Intel® VT features**.

#.  While booting the Host OS kernel, pass the intel_iommu=on kernel command line argument using GRUB.
    When using Intel® DPDK PF driver on host, pass the iommu=pt kernel command line argument in GRUB.

#.  Download qemu-kvm-0.14.0 from
    `http://sourceforge.net/projects/kvm/files/qemu-kvm/ <http://sourceforge.net/projects/kvm/files/qemu-kvm/>`_
    and install it in the Host OS using the following steps:

    When using a recent kernel (2.6.25+) with kvm modules included:

    .. code-block:: console

        tar xzf qemu-kvm-release.tar.gz
        cd qemu-kvm-release
        ./configure --prefix=/usr/local/kvm
        make
        sudo make install
        sudo /sbin/modprobe kvm-intel

    When using an older kernel, or a kernel from a distribution without the kvm modules,
    you must download (from the same link), compile and install the modules yourself:

    .. code-block:: console

        tar xjf kvm-kmod-release.tar.bz2
        cd kvm-kmod-release
        ./configure
        make
        sudo make install
        sudo /sbin/modprobe kvm-intel

    qemu-kvm installs in the /usr/local/bin directory.

    For more details about KVM configuration and usage, please refer to:

    `http://www.linux-kvm.org/page/HOWTO1 <http://www.linux-kvm.org/page/HOWTO1>`_.

#.  Create a Virtual Machine and install Fedora 14 on the Virtual Machine.
    This is referred to as the Guest Operating System (Guest OS).

#.  Download and install the latest ixgbe driver from:

    `http://downloadcenter.intel.com/Detail_Desc.aspx?agr=Y&amp;DwnldID=14687 <http://downloadcenter.intel.com/Detail_Desc.aspx?agr=Y&amp;DwnldID=14687>`_

#.  In the Host OS

    When using Linux kernel ixgbe driver, unload the Linux ixgbe driver and reload it with the max_vfs=2,2 argument:

    .. code-block:: console

        rmmod ixgbe
        "modprobe ixgbe max_vfs=2,2"

    When using DPDK PMD PF driver, insert Intel® DPDK kernel module igb_uio and set the number of VF by sysfs max_vfs:

    .. code-block:: console

        modprobe uio
        insmod igb_uio
        ./dpdk_nic_bind.py -b igb_uio 02:00.0 02:00.1 0e:00.0 0e:00.1
        echo 2 > /sys/bus/pci/devices/0000\:02\:00.0/max_vfs
        echo 2 > /sys/bus/pci/devices/0000\:02\:00.1/max_vfs
        echo 2 > /sys/bus/pci/devices/0000\:0e\:00.0/max_vfs
        echo 2 > /sys/bus/pci/devices/0000\:0e\:00.1/max_vfs

    .. note::

        You need to explicitly specify number of vfs for each port, for example,
        in the command above, it creates two vfs for the first two ixgbe ports.

    Let say we have a machine with four physical ixgbe ports:


        0000:02:00.0

        0000:02:00.1

        0000:0e:00.0

        0000:0e:00.1

    The command above creates two vfs for device 0000:02:00.0:

    .. code-block:: console

        ls -alrt /sys/bus/pci/devices/0000\:02\:00.0/virt*
        lrwxrwxrwx. 1 root root 0 Apr 13 05:40 /sys/bus/pci/devices/0000:02:00.0/ virtfn1 -> ../0000:02:10.2
        lrwxrwxrwx. 1 root root 0 Apr 13 05:40 /sys/bus/pci/devices/0000:02:00.0/ virtfn0 -> ../0000:02:10.0

    It also creates two vfs for device 0000:02:00.1:

    .. code-block:: console

        ls -alrt /sys/bus/pci/devices/0000\:02\:00.1/virt*
        lrwxrwxrwx. 1 root root 0 Apr 13 05:51 /sys/bus/pci/devices/0000:02:00.1/
        virtfn1 -> ../0000:02:10.3
        lrwxrwxrwx. 1 root root 0 Apr 13 05:51 /sys/bus/pci/devices/0000:02:00.1/
        virtfn0 -> ../0000:02:10.1

#.  List the PCI devices connected and notice that the Host OS shows two Physical Functions (traditional ports)
    and four Virtual Functions (two for each port).
    This is the result of the previous step.

#.  Insert the pci_stub module to hold the PCI devices that are freed from the default driver using the following command
    (see http://www.linux-kvm.org/page/How_to_assign_devices_with_VT-d_in_KVM Section 4 for more information):

    .. code-block:: console

        sudo /sbin/modprobe pci-stub

    Unbind the default driver from the PCI devices representing the Virtual Functions.
    A script to perform this action is as follows:

    .. code-block:: console

        echo "8086 10ed" > /sys/bus/pci/drivers/pci-stub/new_id
        echo 0000:08:10.0 > /sys/bus/pci/devices/0000:08:10.0/driver/unbind
        echo 0000:08:10.0 > /sys/bus/pci/drivers/pci-stub/bind

    where, 0000:08:10.0 belongs to the Virtual Function visible in the Host OS.

#.  Now, start the Virtual Machine by running the following command:

    .. code-block:: console

        /usr/local/kvm/bin/qemu-system-x86_64 -m 4096 -smp 4 -boot c -hda lucid.qcow2 -device pci-assign,host=08:10.0

    where:

        — -m = memory to assign

        — -smp = number of smp cores

        — -boot = boot option

        — -hda = virtual disk image

        — -device = device to attach

    .. note::

        — The pci-assign,host=08:10.0 alue indicates that you want to attach a PCI device
        to a Virtual Machine and the respective (Bus:Device.Function)
        numbers should be passed for the Virtual Function to be attached.

        — qemu-kvm-0.14.0 allows a maximum of four PCI devices assigned to a VM,
        but this is qemu-kvm version dependent since qemu-kvm-0.14.1 allows a maximum of five PCI devices.

        — qemu-system-x86_64 also has a -cpu command line option that is used to select the cpu_model
        to emulate in a Virtual Machine. Therefore, it can be used as:

        .. code-block:: console

            /usr/local/kvm/bin/qemu-system-x86_64 -cpu ?

            (to list all available cpu_models)

            /usr/local/kvm/bin/qemu-system-x86_64 -m 4096 -cpu host -smp 4 -boot c -hda lucid.qcow2 -device pci-assign,host=08:10.0

            (to use the same cpu_model equivalent to the host cpu)

        For more information, please refer to: `http://wiki.qemu.org/Features/CPUModels <http://wiki.qemu.org/Features/CPUModels>`_.

#.  Install and run DPDK host app to take  over the Physical Function. Eg.

    .. code-block:: console

        make install T=x86_64-native-linuxapp-gcc
        ./x86_64-native-linuxapp-gcc/app/testpmd -c f -n 4 -- -i

#.  Finally, access the Guest OS using vncviewer with the localhost:5900 port and check the lspci command output in the Guest OS.
    The virtual functions will be listed as available for use.

#.  Configure and install the Intel® DPDK with an x86_64-native-linuxapp-gcc configuration on the Guest OS as normal,
    that is, there is no change to the normal installation procedure.

    .. code-block:: console

        make config T=x86_64-native-linuxapp-gcc O=x86_64-native-linuxapp-gcc
        cd x86_64-native-linuxapp-gcc
        make

.. note::

    If you are unable to compile the Intel® DPDK and you are getting "error: CPU you selected does not support x86-64 instruction set",
    power off the Guest OS and start the virtual machine with the correct -cpu option in the qemu- system-x86_64 command as shown in step 9.
    You must select the best x86_64 cpu_model to emulate or you can select host option if available.

.. note::

    Run the Intel® DPDK l2fwd sample application in the Guest OS with Hugepages enabled.
    For the expected benchmark performance, you must pin the cores from the Guest OS to the Host OS (taskset can be used to do this) and
    you must also look at the PCI Bus layout on the board to ensure you are not running the traffic over the QPI Inteface.

.. note::

    *   The Virtual Machine Manager (the Fedora package name is virt-manager) is a utility for virtual machine management
        that can also be used to create, start, stop and delete virtual machines.
        If this option is used, step 2 and 6 in the instructions provided will be different.

    *   virsh, a command line utility for virtual machine management,
        can also be used to bind and unbind devices to a virtual machine in Ubuntu.
        If this option is used, step 6 in the instructions provided will be different.

    *   The Virtual Machine Monitor (see Figure 11) is equivalent to a Host OS with KVM installed as described in the instructions.

.. _pg_figure_11:

**Figure 11. Performance Benchmark Setup**

.. image25_png has been renamed

|perf_benchmark|

Intel® DPDK SR-IOV PMD PF/VF Driver Usage Model
-----------------------------------------------

Fast Host-based Packet Processing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Software Defined Network (SDN) trends are demanding fast host-based packet handling.
In a virtualization environment,
the Intel® DPDK VF PMD driver performs the same throughput result as a non-VT native environment.

With such host instance fast packet processing, lots of services such as filtering, QoS,
DPI can be offloaded on the host fast path.

shows the scenario where some VMs directly communicate externally via a VFs,
while others connect to a virtual switch and share the same uplink bandwidth.

.. _pg_figure_12:

**Figure 12. Fast Host-based Packet Processing**

.. image26_png has been renamed

|fast_pkt_proc|

SR-IOV (PF/VF) Approach for Inter-VM Communication
--------------------------------------------------

Inter-VM data communication is one of the traffic bottle necks in virtualization platforms.
SR-IOV device assignment helps a VM to attach the real device, taking advantage of the bridge in the NIC.
So VF-to-VF traffic within the same physical port (VM0<->VM1) have hardware acceleration.
However, when VF crosses physical ports (VM0<->VM2), there is no such hardware bridge.
In this case, the Intel® DPDK PMD PF driver provides host forwarding between such VMs.

Figure 13 shows an example.
In this case an update of the MAC address lookup tables in both the NIC and host Intel® DPDK application is required.

In the NIC, writing the destination of a MAC address belongs to another cross device VM to the PF specific pool.
So when a packet comes in, its destination MAC address will match and forward to the host Intel® DPDK PMD application.

In the host Intel® DPDK application, the behavior is similar to L2 forwarding,
that is, the packet is forwarded to the correct PF pool.
The SR-IOV NIC switch forwards the packet to a specific VM according to the MAC destination address
which belongs to the destination VF on the VM.

.. _pg_figure_13:

**Figure 13. Inter-VM Communication**

.. image27_png has been renamed

|inter_vm_comms|

.. |perf_benchmark| image:: img/perf_benchmark.png

.. |single_port_nic| image:: img/single_port_nic.png

.. |inter_vm_comms| image:: img/inter_vm_comms.png

.. |fast_pkt_proc| image:: img/fast_pkt_proc.png
