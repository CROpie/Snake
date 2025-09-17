export type Position = {
    x: number
    y: number
}

export type PlayerData = {
    player: string
    chain: Position[]
}

export type GameStateResponse = {
    type: "gameState"
    gameState: PlayerData[]
}

export type WallResponse = {
    type: "walls"
    walls: Position[]
}

export type SnakeResponse = GameStateResponse | WallResponse

export interface CanvasRenderService {
    init: () => void
    render: (response: PlayerData[]) => void
    setWalls: (WALLS: Position[]) => void
}

export type Config = {
    WS_HOST: string
}