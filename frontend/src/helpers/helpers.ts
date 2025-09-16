import type { Config } from "../types"

export function assignEle<T extends HTMLElement>(elementId: string): T {
    const element = document.getElementById(elementId)
    if (!element) throw new Error(`no element with id ${elementId}`)
    return element as T
}

export function initCtx(canvas: HTMLCanvasElement): CanvasRenderingContext2D {
    const ctx = canvas.getContext("2d")
    if (!ctx) throw new Error("2D context not supported")
    return ctx
}

export async function loadConfig(): Promise<Config> {
    const response = await fetch("dist/config.json");
    return response.json();
}