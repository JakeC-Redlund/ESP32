#!/usr/bin/env node
"use strict";

const childProcess = require("child_process");
const fs = require("fs");
const http = require("http");
const os = require("os");
const path = require("path");

const ROOT = path.resolve(__dirname, "..");
const DEFAULT_ARTIFACT = path.join(ROOT, "build", "github-pages");
const LOAD_TIMEOUT_MS = 15000;

const EXPECTED_WEBP_DIMENSIONS = {
  "assets/blueprints/system-overview.webp": [1600, 1000],
  "assets/blueprints/safety-proof-ladder.webp": [1600, 1000],
  "assets/blueprints/prototype-evidence-map.webp": [1600, 1000],
  "assets/blueprints/low-voltage-review-sequence.webp": [1600, 1000],
  "assets/blueprints/pin-pressure-map.webp": [1600, 1000],
  "assets/workbench/hero-workbench.webp": [1800, 1080],
  "assets/workbench/rd-loop-backplate.webp": [1600, 1000],
  "assets/workbench/admin-hmi-backplate.webp": [1600, 1000],
};

const VIEWPORTS = [
  { name: "desktop", width: 1280, height: 900, deviceScaleFactor: 1, mobile: false },
  { name: "mobile", width: 390, height: 844, deviceScaleFactor: 2, mobile: true },
];

const PAGES = [
  {
    path: "index.html",
    requiredText: [
      "ESP32 four-relay workbench",
      "Open Prototype Build Packet",
      "Candidate GPIO25",
    ],
  },
  {
    path: "prototype.html",
    requiredText: [
      "Prototype Build Packet",
      "No blockers means findability",
      "Public Markdown content",
      "does not approve final wiring",
    ],
    requiredImages: [
      "assets/blueprints/prototype-evidence-map.webp",
      "assets/blueprints/low-voltage-review-sequence.webp",
      "assets/blueprints/pin-pressure-map.webp",
    ],
    checkFocus: true,
  },
  {
    path: "blueprints.html",
    requiredText: [
      "Conceptual system map, not wiring instructions",
      "Safety evidence ladder",
      "Relay/load wiring",
    ],
  },
  {
    path: "quality.html",
    requiredText: [
      "What this public artifact checks",
      "Manifest, link, content, and hash audit",
      "not electrical safety testing",
    ],
  },
  {
    path: "demos/admin-hmi/index.html",
    requiredText: [
      "Static artifact",
      "Static non-operational demo",
      "Relay commands blocked",
    ],
  },
];

const MIME_TYPES = {
  ".css": "text/css; charset=utf-8",
  ".html": "text/html; charset=utf-8",
  ".js": "application/javascript; charset=utf-8",
  ".json": "application/json; charset=utf-8",
  ".md": "text/markdown; charset=utf-8",
  ".webp": "image/webp",
};

class CdpSession {
  constructor(webSocketUrl) {
    this.webSocketUrl = webSocketUrl;
    this.ws = new WebSocket(webSocketUrl);
    this.nextId = 1;
    this.pending = new Map();
    this.listeners = new Map();
  }

  async connect() {
    if (this.ws.readyState === WebSocket.OPEN) {
      return;
    }
    await new Promise((resolve, reject) => {
      const onOpen = () => {
        this.ws.removeEventListener("error", onError);
        resolve();
      };
      const onError = (event) => {
        this.ws.removeEventListener("open", onOpen);
        reject(new Error(`CDP websocket failed: ${event.message || this.webSocketUrl}`));
      };
      this.ws.addEventListener("open", onOpen, { once: true });
      this.ws.addEventListener("error", onError, { once: true });
    });
    this.ws.addEventListener("message", (event) => this.dispatch(event));
  }

  dispatch(event) {
    const raw = typeof event.data === "string" ? event.data : Buffer.from(event.data).toString("utf-8");
    const message = JSON.parse(raw);
    if (message.id && this.pending.has(message.id)) {
      const { resolve, reject } = this.pending.get(message.id);
      this.pending.delete(message.id);
      if (message.error) {
        reject(new Error(`${message.error.message || "CDP error"} (${message.error.code})`));
      } else {
        resolve(message.result || {});
      }
      return;
    }
    if (!message.method || !this.listeners.has(message.method)) {
      return;
    }
    for (const listener of this.listeners.get(message.method)) {
      listener(message.params || {});
    }
  }

  on(method, listener) {
    if (!this.listeners.has(method)) {
      this.listeners.set(method, new Set());
    }
    this.listeners.get(method).add(listener);
    return () => this.listeners.get(method).delete(listener);
  }

  waitFor(method, predicate, timeoutMs) {
    return new Promise((resolve, reject) => {
      const timer = setTimeout(() => {
        unsubscribe();
        reject(new Error(`Timed out waiting for ${method}`));
      }, timeoutMs);
      const unsubscribe = this.on(method, (params) => {
        if (!predicate(params)) {
          return;
        }
        clearTimeout(timer);
        unsubscribe();
        resolve(params);
      });
    });
  }

  send(method, params = {}) {
    if (this.ws.readyState !== WebSocket.OPEN) {
      return Promise.reject(new Error(`CDP websocket is not open for ${method}`));
    }
    const id = this.nextId++;
    const payload = JSON.stringify({ id, method, params });
    return new Promise((resolve, reject) => {
      this.pending.set(id, { resolve, reject });
      this.ws.send(payload);
    });
  }

  close() {
    if (this.ws.readyState === WebSocket.OPEN || this.ws.readyState === WebSocket.CONNECTING) {
      this.ws.close();
    }
  }
}

function parseArgs() {
  const artifact = process.argv[2] ? path.resolve(process.argv[2]) : DEFAULT_ARTIFACT;
  return { artifact };
}

function assertArtifact(artifact) {
  const prototype = path.join(artifact, "prototype.html");
  if (!fs.existsSync(prototype)) {
    throw new Error(`artifact missing prototype.html: ${artifact}`);
  }
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function commandPath(command) {
  const result = childProcess.spawnSync("which", [command], {
    encoding: "utf-8",
    stdio: ["ignore", "pipe", "ignore"],
  });
  if (result.status === 0 && result.stdout.trim()) {
    return result.stdout.trim().split(/\r?\n/)[0];
  }
  return null;
}

function findPlaywrightChromes() {
  const cacheRoot = path.join(os.homedir(), ".cache", "ms-playwright");
  if (!fs.existsSync(cacheRoot)) {
    return [];
  }
  const candidates = [];
  for (const entry of fs.readdirSync(cacheRoot).sort().reverse()) {
    const chrome = path.join(cacheRoot, entry, "chrome-linux64", "chrome");
    if (fs.existsSync(chrome)) {
      candidates.push(chrome);
    }
  }
  return candidates;
}

function findChromeExecutable() {
  const candidates = [
    process.env.CHROME_PATH,
    process.env.BROWSER,
    commandPath("google-chrome"),
    commandPath("google-chrome-stable"),
    commandPath("chromium"),
    commandPath("chromium-browser"),
    commandPath("microsoft-edge"),
    "/usr/bin/google-chrome",
    "/usr/bin/google-chrome-stable",
    "/usr/bin/chromium",
    "/usr/bin/chromium-browser",
    "/opt/google/chrome/chrome",
    ...findPlaywrightChromes(),
  ].filter(Boolean);

  for (const candidate of candidates) {
    if (fs.existsSync(candidate)) {
      return candidate;
    }
  }
  return null;
}

function startStaticServer(root) {
  const server = http.createServer((req, res) => {
    const requestUrl = new URL(req.url || "/", "http://127.0.0.1");
    let relativePath = decodeURIComponent(requestUrl.pathname).replace(/^\/+/, "");
    if (!relativePath || relativePath.endsWith("/")) {
      relativePath += "index.html";
    }
    const target = path.resolve(root, relativePath);
    if (!(target === root || target.startsWith(`${root}${path.sep}`))) {
      res.writeHead(403, { "content-type": "text/plain; charset=utf-8" });
      res.end("forbidden");
      return;
    }
    fs.stat(target, (statError, stat) => {
      if (statError) {
        res.writeHead(404, { "content-type": "text/plain; charset=utf-8" });
        res.end("not found");
        return;
      }
      const file = stat.isDirectory() ? path.join(target, "index.html") : target;
      const contentType = MIME_TYPES[path.extname(file).toLowerCase()] || "application/octet-stream";
      res.writeHead(200, { "content-type": contentType });
      fs.createReadStream(file).pipe(res);
    });
  });
  return new Promise((resolve) => {
    server.listen(0, "127.0.0.1", () => {
      resolve({
        server,
        port: server.address().port,
        close: () => new Promise((done) => server.close(done)),
      });
    });
  });
}

async function launchChrome(executable) {
  const userDataDir = fs.mkdtempSync(path.join(os.tmpdir(), "esp32-pages-browser-"));
  let stderr = "";
  const chrome = childProcess.spawn(executable, [
    "--headless=new",
    "--remote-debugging-port=0",
    `--user-data-dir=${userDataDir}`,
    "--no-first-run",
    "--no-default-browser-check",
    "--disable-background-networking",
    "--disable-dev-shm-usage",
    "--disable-extensions",
    "--disable-gpu",
    "--metrics-recording-only",
    "--no-sandbox",
    "about:blank",
  ], { stdio: ["ignore", "ignore", "pipe"] });
  chrome.stderr.on("data", (chunk) => {
    stderr = `${stderr}${chunk.toString("utf-8")}`.slice(-12000);
  });

  const activePortFile = path.join(userDataDir, "DevToolsActivePort");
  const deadline = Date.now() + LOAD_TIMEOUT_MS;
  while (Date.now() < deadline) {
    if (chrome.exitCode !== null) {
      throw new Error(`Chrome exited before DevTools was ready: ${stderr.trim()}`);
    }
    if (fs.existsSync(activePortFile)) {
      const [portText] = fs.readFileSync(activePortFile, "utf-8").trim().split(/\r?\n/);
      const port = Number(portText);
      if (Number.isInteger(port) && port > 0) {
        return { process: chrome, port, userDataDir, stderr: () => stderr };
      }
    }
    await sleep(100);
  }
  throw new Error(`Timed out waiting for Chrome DevTools port: ${stderr.trim()}`);
}

function waitForChromeExit(chrome, timeoutMs) {
  if (chrome.exitCode !== null || chrome.signalCode !== null) {
    return Promise.resolve(true);
  }
  return new Promise((resolve) => {
    const onExit = () => {
      clearTimeout(timer);
      resolve(true);
    };
    const timer = setTimeout(() => {
      chrome.removeListener("exit", onExit);
      resolve(false);
    }, timeoutMs);
    chrome.once("exit", onExit);
  });
}

async function stopChrome(handle) {
  if (!handle) {
    return;
  }
  if (handle.process.exitCode === null && handle.process.signalCode === null) {
    handle.process.kill("SIGTERM");
    if (!(await waitForChromeExit(handle.process, 2000))) {
      handle.process.kill("SIGKILL");
      if (!(await waitForChromeExit(handle.process, 2000))) {
        throw new Error(`Owned Chrome did not exit; retained profile: ${handle.userDataDir}`);
      }
    }
  }
  fs.rmSync(handle.userDataDir, {
    recursive: true,
    force: true,
    maxRetries: 10,
    retryDelay: 100,
  });
}

async function createTarget(port, targetUrl) {
  const endpoint = `http://127.0.0.1:${port}/json/new?${encodeURIComponent(targetUrl)}`;
  let response = await fetch(endpoint, { method: "PUT" });
  if (!response.ok && (response.status === 404 || response.status === 405)) {
    response = await fetch(endpoint);
  }
  if (!response.ok) {
    throw new Error(`Chrome target create failed: ${response.status} ${response.statusText}`);
  }
  return response.json();
}

async function closeTarget(port, targetId) {
  if (!targetId) {
    return;
  }
  try {
    await fetch(`http://127.0.0.1:${port}/json/close/${encodeURIComponent(targetId)}`);
  } catch (_error) {
    return;
  }
}

async function evaluate(session, expression) {
  const result = await session.send("Runtime.evaluate", {
    expression,
    awaitPromise: true,
    returnByValue: true,
  });
  if (result.exceptionDetails) {
    const description = result.exceptionDetails.exception
      ? result.exceptionDetails.exception.description
      : result.exceptionDetails.text;
    throw new Error(description || "Runtime.evaluate failed");
  }
  return result.result ? result.result.value : undefined;
}

function browserAudit(config) {
  const visibleText = document.body ? document.body.innerText : "";
  const normalizedVisibleText = visibleText.toLocaleLowerCase("en-US");
  const missingText = config.requiredText.filter(
    (token) => !normalizedVisibleText.includes(token.toLocaleLowerCase("en-US"))
  );
  const documentElement = document.documentElement;
  const body = document.body;
  const clientWidth = Math.max(documentElement.clientWidth, window.innerWidth || 0);
  const scrollWidth = Math.max(documentElement.scrollWidth, body ? body.scrollWidth : 0);
  const bodyRect = body ? body.getBoundingClientRect() : { width: 0, height: 0 };
  const images = Array.from(document.images)
    .filter((image) => (image.currentSrc || image.src || "").includes(".webp"))
    .map((image) => {
      const sourceUrl = new URL(image.currentSrc || image.src, window.location.href);
      return {
        path: decodeURIComponent(sourceUrl.pathname.replace(/^\/+/, "")),
        complete: image.complete,
        naturalWidth: image.naturalWidth,
        naturalHeight: image.naturalHeight,
        attrWidth: image.getAttribute("width"),
        attrHeight: image.getAttribute("height"),
      };
    });
  return {
    title: document.title,
    missingText,
    renderBox: { width: bodyRect.width, height: bodyRect.height },
    overflow: {
      clientWidth,
      scrollWidth,
      hasHorizontalOverflow: scrollWidth > clientWidth + 1,
    },
    images,
  };
}

function focusState() {
  const element = document.activeElement;
  if (!element || element === document.body || element === document.documentElement) {
    return { selector: "document", visible: false, reason: "no focused element" };
  }
  const style = window.getComputedStyle(element);
  const rect = element.getBoundingClientRect();
  const outlineWidth = Number.parseFloat(style.outlineWidth) || 0;
  const borderWidth = Number.parseFloat(style.borderWidth) || 0;
  const visibleOutline =
    style.outlineStyle !== "none" &&
    outlineWidth >= 1 &&
    style.outlineColor !== "rgba(0, 0, 0, 0)";
  const visibleShadow = Boolean(style.boxShadow && style.boxShadow !== "none");
  const visibleBorder =
    element.matches(":focus-visible") &&
    style.borderStyle !== "none" &&
    borderWidth >= 1;
  const text = (element.textContent || element.getAttribute("aria-label") || "")
    .replace(/\s+/g, " ")
    .trim()
    .slice(0, 80);
  return {
    selector: element.id ? `#${element.id}` : `${element.tagName.toLowerCase()} ${text}`.trim(),
    tag: element.tagName.toLowerCase(),
    text,
    focusVisible: element.matches(":focus-visible"),
    outlineStyle: style.outlineStyle,
    outlineWidth: style.outlineWidth,
    boxShadow: style.boxShadow,
    rect: { width: rect.width, height: rect.height },
    visible:
      rect.width > 0 &&
      rect.height > 0 &&
      (visibleOutline || visibleShadow || visibleBorder),
  };
}

async function dispatchTab(session) {
  const params = {
    key: "Tab",
    code: "Tab",
    windowsVirtualKeyCode: 9,
    nativeVirtualKeyCode: 9,
  };
  await session.send("Input.dispatchKeyEvent", { ...params, type: "rawKeyDown" });
  await session.send("Input.dispatchKeyEvent", { ...params, type: "keyUp" });
}

async function checkFocusVisibility(session) {
  await evaluate(session, "window.focus(); window.scrollTo(0, 0); true;");
  const states = [];
  const seen = new Set();
  for (let index = 0; index < 8; index += 1) {
    await dispatchTab(session);
    await sleep(80);
    const state = await evaluate(session, `(${focusState.toString()})()`);
    if (!seen.has(state.selector)) {
      seen.add(state.selector);
      states.push(state);
    }
  }
  return states;
}

function sameOrigin(baseUrl, candidateUrl) {
  try {
    return new URL(candidateUrl).origin === new URL(baseUrl).origin;
  } catch (_error) {
    return false;
  }
}

function auditImages(pageSpec, pageAudit) {
  const failures = [];
  const seen = new Set();
  for (const image of pageAudit.images) {
    const expected = EXPECTED_WEBP_DIMENSIONS[image.path];
    if (!expected) {
      failures.push(`unexpected WebP image ${image.path}`);
      continue;
    }
    seen.add(image.path);
    if (!image.complete || image.naturalWidth === 0 || image.naturalHeight === 0) {
      failures.push(`WebP image did not decode: ${image.path}`);
      continue;
    }
    if (image.naturalWidth !== expected[0] || image.naturalHeight !== expected[1]) {
      failures.push(
        `WebP dimensions mismatch for ${image.path}: expected ${expected[0]}x${expected[1]}, got ${image.naturalWidth}x${image.naturalHeight}`
      );
    }
  }
  for (const required of pageSpec.requiredImages || []) {
    if (!seen.has(required)) {
      failures.push(`required WebP image not rendered: ${required}`);
    }
  }
  return failures;
}

async function checkPage(chromePort, baseUrl, pageSpec, viewport) {
  const pageUrl = new URL(pageSpec.path, baseUrl).toString();
  const target = await createTarget(chromePort, "about:blank");
  const session = new CdpSession(target.webSocketDebuggerUrl);
  const failures = [];
  const requestUrls = new Map();
  const consoleErrors = [];
  const requestFailures = [];
  try {
    await session.connect();
    session.on("Network.requestWillBeSent", (event) => {
      requestUrls.set(event.requestId, event.request.url);
    });
    session.on("Network.responseReceived", (event) => {
      const response = event.response || {};
      if (sameOrigin(baseUrl, response.url) && response.status >= 400) {
        requestFailures.push(`${response.url} returned ${response.status}`);
      }
    });
    session.on("Network.loadingFailed", (event) => {
      const requestUrl = requestUrls.get(event.requestId) || "";
      if (sameOrigin(baseUrl, requestUrl)) {
        requestFailures.push(`${requestUrl} failed: ${event.errorText || "unknown"}`);
      }
    });
    session.on("Runtime.consoleAPICalled", (event) => {
      if (!["error", "assert"].includes(event.type)) {
        return;
      }
      const message = (event.args || [])
        .map((arg) => arg.value || arg.description || arg.unserializableValue || "")
        .join(" ");
      consoleErrors.push(`${event.type}: ${message}`.trim());
    });
    session.on("Runtime.exceptionThrown", (event) => {
      const detail = event.exceptionDetails || {};
      const exception = detail.exception || {};
      consoleErrors.push(exception.description || detail.text || "runtime exception");
    });

    await session.send("Page.enable");
    await session.send("Runtime.enable");
    await session.send("Network.enable");
    await session.send("Emulation.setDeviceMetricsOverride", {
      width: viewport.width,
      height: viewport.height,
      deviceScaleFactor: viewport.deviceScaleFactor,
      mobile: viewport.mobile,
      screenWidth: viewport.width,
      screenHeight: viewport.height,
    });
    const loadEvent = session.waitFor("Page.loadEventFired", () => true, LOAD_TIMEOUT_MS);
    await session.send("Page.navigate", { url: pageUrl });
    await loadEvent;
    await evaluate(session, "document.fonts && document.fonts.ready ? document.fonts.ready.then(() => true) : true;");
    await sleep(350);

    const pageAudit = await evaluate(
      session,
      `(${browserAudit.toString()})(${JSON.stringify({
        requiredText: pageSpec.requiredText,
      })})`
    );
    if (pageAudit.renderBox.width < 1 || pageAudit.renderBox.height < 1) {
      failures.push(`page did not render a visible body box: ${JSON.stringify(pageAudit.renderBox)}`);
    }
    for (const token of pageAudit.missingText) {
      failures.push(`visible text missing: ${token}`);
    }
    if (pageAudit.overflow.hasHorizontalOverflow) {
      failures.push(
        `horizontal overflow: scrollWidth ${pageAudit.overflow.scrollWidth} > clientWidth ${pageAudit.overflow.clientWidth}`
      );
    }
    failures.push(...auditImages(pageSpec, pageAudit));

    if (pageSpec.checkFocus) {
      const focusStates = await checkFocusVisibility(session);
      const visibleFocusCount = focusStates.filter((state) => state.visible).length;
      if (visibleFocusCount < 3) {
        failures.push(`keyboard focus visibility covered only ${visibleFocusCount} target(s)`);
      }
    }

    await sleep(250);
    failures.push(...consoleErrors.map((message) => `console error: ${message}`));
    failures.push(...requestFailures.map((message) => `same-origin request failure: ${message}`));
  } finally {
    session.close();
    await closeTarget(chromePort, target.id);
  }
  return {
    page: pageSpec.path,
    viewport: viewport.name,
    failures,
  };
}

async function main() {
  const { artifact } = parseArgs();
  assertArtifact(artifact);
  const chromeExecutable = findChromeExecutable();
  if (!chromeExecutable) {
    throw new Error(
      "Chrome executable not found. Set CHROME_PATH or install google-chrome/chromium; browser QA fails closed."
    );
  }

  const server = await startStaticServer(artifact);
  let chrome = null;
  const failures = [];
  try {
    chrome = await launchChrome(chromeExecutable);
    const baseUrl = `http://127.0.0.1:${server.port}/`;
    for (const pageSpec of PAGES) {
      for (const viewport of VIEWPORTS) {
        const result = await checkPage(chrome.port, baseUrl, pageSpec, viewport);
        for (const failure of result.failures) {
          failures.push(`${result.page} ${result.viewport}: ${failure}`);
        }
      }
    }
  } finally {
    await stopChrome(chrome);
    await server.close();
  }

  if (failures.length) {
    throw new Error(`Prototype Build Packet browser QA failed:\n- ${failures.join("\n- ")}`);
  }
  console.log(`PASS: Prototype Build Packet browser QA succeeded for ${artifact}`);
}

main().catch((error) => {
  console.error(`FAIL: ${error.message}`);
  process.exit(1);
});
