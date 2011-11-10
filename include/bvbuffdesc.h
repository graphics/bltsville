/*
 * bvbuffdesc.h
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 *
 * This file is part of BLTsville, an open application programming interface
 * (API) for accessing 2-D software or hardware implementations.
 *
 * This work is licensed under the Creative Commons Attribution-NoDerivs 3.0
 * Unported License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nd/3.0/ or send a letter to
 * Creative Commons, 444 Castro Street, Suite 900, Mountain View, California,
 * 94041, USA.
 */

#ifndef BVBUFFDESC_H
#define BVBUFFDESC_H

/*
 * bvbuffmap - This is a private structure used by BLTsville
 * implementations to manage resources associated with a buffer.  A pointer
 * to this is returned from bv_map() and used in subsequent bv_blt() and
 * bv_unmap() calls.
 */
struct bvbuffmap;

/*
 * bvbuffdesc - This structure is used to specify the buffer parameters
 * in a call to bv_map().
 */
struct bvbuffdesc {
	unsigned int structsize;	/* used to identify struct version */
	void *virtaddr;			/* virtual ptr to start of buffer */
	unsigned long length;		/* length of the buffer in bytes */
	struct bvbuffmap *map;	/* resource(s) associated w/buffer */
};

#endif /* BVBUFFDESC_H */
