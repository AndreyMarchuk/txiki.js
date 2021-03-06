
/*
 * QuickJS libuv bindings
 *
 * Copyright (c) 2019-present Saúl Ibarra Corretgé <s@saghul.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TJS_H
#define TJS_H

#include <quickjs.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "tjs_export.h"
#include "utils.h"

typedef struct TJSRuntime TJSRuntime;

typedef struct TJSRunOptions {
    bool abort_on_unhandled_rejection;
    size_t stack_size;
} TJSRunOptions;

TJS_EXPORT void TJS_DefaultOptions(TJSRunOptions *options);
TJS_EXPORT TJSRuntime *TJS_NewRuntime(void);
TJS_EXPORT TJSRuntime *TJS_NewRuntimeOptions(TJSRunOptions *options);
TJS_EXPORT void TJS_FreeRuntime(TJSRuntime *qrt);
TJS_EXPORT void TJS_SetupArgs(int argc, char **argv);
TJS_EXPORT JSContext *TJS_GetJSContext(TJSRuntime *qrt);
TJS_EXPORT TJSRuntime *TJS_GetRuntime(JSContext *ctx);
TJS_EXPORT JSRuntime *TJS_GetJSRuntime(JSContext *ctx);
TJS_EXPORT JSRuntime *TJS_GetJSRuntimeFromTJS(TJSRuntime *qrt);
TJS_EXPORT void TJS_Run(TJSRuntime *qrt);
TJS_EXPORT void TJS_Stop(TJSRuntime *qrt);
TJS_EXPORT JSValue TJS_EvalFile(JSContext *ctx, const char *filename, int eval_flags, bool is_main, char *override_filename);
TJS_EXPORT void TJS_RunRepl(JSContext *ctx);

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif
