#include <stdio.h>
#include <stddef.h>
#include "events.h"
#include "tofi.h"
#include "wlr-layer-shell-unstable-v1.h"

static void escape_json_string(const char *in, char *out, size_t out_size)
{
	size_t j = 0;
	for (size_t i = 0; in[i] && j < out_size - 2; i++) {
		switch (in[i]) {
			case '"':
				out[j++] = '\\';
				out[j++] = '"';
				break;
			case '\\':
				out[j++] = '\\';
				out[j++] = '\\';
				break;
			case '\n':
				out[j++] = '\\';
				out[j++] = 'n';
				break;
			case '\r':
				out[j++] = '\\';
				out[j++] = 'r';
				break;
			case '\t':
				out[j++] = '\\';
				out[j++] = 't';
				break;
			default:
				out[j++] = in[i];
				break;
		}
	}
	out[j] = '\0';
}

void event_open(struct tofi *tofi)
{
	if (!tofi->stream_events) {
		return;
	}

	const char *mode;
	switch (tofi->window.entry.mode) {
		case TOFI_MODE_RUN:
			mode = "run";
			break;
		case TOFI_MODE_DRUN:
			mode = "drun";
			break;
		default:
			mode = "plain";
			break;
	}

	/* Calculate window position based on anchor and margins */
	uint32_t anchor = tofi->anchor;
	int32_t win_x, win_y;
	int32_t out_w = tofi->output_width;
	int32_t out_h = tofi->output_height;
	int32_t win_w = tofi->window.width;
	int32_t win_h = tofi->window.height;

	/* Horizontal position */
	if ((anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT) &&
	    (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT)) {
		/* Centered horizontally */
		win_x = (out_w - win_w) / 2;
	} else if (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT) {
		win_x = tofi->window.margin_left;
	} else if (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT) {
		win_x = out_w - win_w - tofi->window.margin_right;
	} else {
		win_x = (out_w - win_w) / 2;
	}

	/* Vertical position */
	if ((anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP) &&
	    (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM)) {
		/* Centered vertically */
		win_y = (out_h - win_h) / 2;
	} else if (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP) {
		win_y = tofi->window.margin_top;
	} else if (anchor & ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM) {
		win_y = out_h - win_h - tofi->window.margin_bottom;
	} else {
		win_y = (out_h - win_h) / 2;
	}

	fprintf(stderr, "{\"event\":\"open\",\"mode\":\"%s\",\"items\":%zu,"
			"\"window\":{\"x\":%d,\"y\":%d,\"width\":%u,\"height\":%u},"
			"\"output\":{\"width\":%d,\"height\":%d}}\n",
			mode, tofi->window.entry.results.count,
			win_x, win_y, win_w, win_h,
			out_w, out_h);
	fflush(stderr);
}

void event_input(struct tofi *tofi, const char *input, size_t result_count)
{
	if (!tofi->stream_events) {
		return;
	}

	char escaped[4096];
	escape_json_string(input, escaped, sizeof(escaped));
	fprintf(stderr, "{\"event\":\"input\",\"value\":\"%s\",\"results\":%zu}\n",
			escaped, result_count);
	fflush(stderr);
}

void event_select(struct tofi *tofi, uint32_t index, const char *value)
{
	if (!tofi->stream_events) {
		return;
	}

	char escaped[4096];
	escape_json_string(value, escaped, sizeof(escaped));
	fprintf(stderr, "{\"event\":\"select\",\"index\":%u,\"value\":\"%s\"}\n",
			index, escaped);
	fflush(stderr);
}

void event_submit(struct tofi *tofi, uint32_t index, const char *value)
{
	if (!tofi->stream_events) {
		return;
	}

	char escaped[4096];
	escape_json_string(value, escaped, sizeof(escaped));
	fprintf(stderr, "{\"event\":\"submit\",\"index\":%u,\"value\":\"%s\"}\n",
			index, escaped);
	fflush(stderr);
}

void event_close(struct tofi *tofi, const char *reason)
{
	if (!tofi->stream_events) {
		return;
	}

	fprintf(stderr, "{\"event\":\"close\",\"reason\":\"%s\"}\n", reason);
	fflush(stderr);
}
