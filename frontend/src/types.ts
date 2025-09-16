export type PlayerPos = {
    name: string
    x: number
    y: number
}

export interface CanvasRenderService {
    init: () => void
    render: (response: PlayerPos[]) => void
}

export type Config = {
    WS_HOST: string
}