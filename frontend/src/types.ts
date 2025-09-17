export type Chain = {
    x: number
    y: number
}

export type PlayerData = {
    player: string
    chain: Chain[]
}

export interface CanvasRenderService {
    init: () => void
    render: (response: PlayerData[]) => void
}

export type Config = {
    WS_HOST: string
}