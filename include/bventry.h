/*
 * bventry.h
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef BVENTRY_H
#define BVENTRY_H

/* Forward declarations */
struct bvbuffdesc;
struct bvbltparams;
struct bvcopparams;
/*
 * bv_*() - These are the API calls for BLTsville.  The client needs to
 * import these from the kernel module.  Normally this will be via a call
 * to a function like bv*_init(&bventry).
 */
typedef enum bverror (*BVFN_MAP)(struct bvbuffdesc *buffdesc);
typedef enum bverror (*BVFN_BLT)(struct bvbltparams *bltparms);
typedef enum bverror (*BVFN_UNMAP)(struct bvbuffdesc *buffdesc);
typedef enum bverror (*BVFN_CACHE)(struct bvcopparams *copparams);

struct bventry {
	unsigned int structsize;	/* used to identify struct version */
	BVFN_MAP bv_map;
	BVFN_UNMAP bv_unmap;
	BVFN_BLT bv_blt;
	BVFN_CACHE bv_cache;
};

#endif /* BVENTRY_H */
