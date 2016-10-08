/*
 * Copyright (c) 2015, Henry Corrigan-Gibbs
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */


#include <balloon.h>

#include "compress.h"
#include "constants.h"
#include "errors.h"
#include "options.h"

int 
options_validate (struct balloon_options *opts)
{
  if (opts->t_cost < TCOST_MIN)
    opts->t_cost = TCOST_MIN;
  if (opts->m_cost < MCOST_MIN)
    opts->m_cost = MCOST_MIN;
  if (opts->m_cost >= MCOST_MAX)
    return ERROR_MCOST_TOO_BIG;

  if (opts->n_threads > THREADS_MAX)
    return ERROR_NTHREADS_TOO_BIG;

  if (opts->n_threads > 1) {
    return ERROR_INCOMPATIBLE_OPTIONS;
  }

  const uint64_t n_blocks = options_n_blocks (opts);
  const uint16_t block_size = compress_block_size (opts->comp);
  if (n_blocks > UINT64_MAX / block_size)
    return ERROR_MCOST_TOO_BIG;

  return ERROR_NONE;
}

uint64_t
options_n_blocks (const struct balloon_options *opts)
{
  const uint16_t bsize = compress_block_size (opts->comp);
  uint64_t ret = opts->m_cost / bsize;
  return (ret < BLOCKS_MIN) ? BLOCKS_MIN : ret;
}

uint8_t 
options_n_neighbors (const struct balloon_options *opts)
{
  return (opts->t_cost == 1) ? 1 : 3;
}

