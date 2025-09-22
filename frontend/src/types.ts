export type Position = {
    x: number
    y: number
}

export type PlayerData = {
    player: string
    colour: number
    lives: number
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
    init: ({IMAGE_PATH}: {IMAGE_PATH: string}) => void
    render: (response: PlayerData[]) => void
    setWalls: (WALLS: Position[]) => void
}

export type Config = {
    WS_HOST: string
    LOBBY_URL: string
    AUTH_BASE_URL: string
    IMAGE_PATH: string
}