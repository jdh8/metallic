;; This file is part of Metallic, a runtime library for WebAssembly.
;;
;; Copyright (C) 2017 Chen-Pang He <chen.pang.he@jdh8.org>
;;
;; This Source Code Form is subject to the terms of the Mozilla
;; Public License v. 2.0. If a copy of the MPL was not distributed
;; with this file, You can obtain one at http://mozilla.org/MPL/2.0/

(module
 (type $0 (func))
 (import "env" "_start" (func $_start))
 (start $_start)
)
