/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2015-2018 Intel Corporation
 */

#ifndef _QAT_COMP_H_
#define _QAT_COMP_H_

#ifdef RTE_LIBRTE_COMPRESSDEV

#include <rte_compressdev.h>
#include <rte_compressdev_pmd.h>

#include "qat_common.h"
#include "icp_qat_hw.h"
#include "icp_qat_fw_comp.h"
#include "icp_qat_fw_la.h"

enum qat_comp_request_type {
	QAT_COMP_REQUEST_FIXED_COMP_STATELESS,
	QAT_COMP_REQUEST_DYNAMIC_COMP_STATELESS,
	QAT_COMP_REQUEST_DECOMPRESS,
	REQ_COMP_END
};


struct qat_comp_xform {
	struct icp_qat_fw_comp_req qat_comp_req_tmpl;
	enum qat_comp_request_type qat_comp_request_type;
	enum rte_comp_checksum_type checksum_type;
};

int
qat_comp_build_request(void *in_op, uint8_t *out_msg, void *op_cookie,
		       enum qat_device_gen qat_dev_gen __rte_unused);

int
qat_comp_process_response(void **op, uint8_t *resp);


int
qat_comp_private_xform_create(struct rte_compressdev *dev,
			      const struct rte_comp_xform *xform,
			      void **private_xform);

int
qat_comp_private_xform_free(struct rte_compressdev *dev, void *private_xform);

unsigned int
qat_comp_xform_size(void);

#endif
#endif