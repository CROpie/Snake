import { assignEle, initCtx, loadConfig } from "./helpers/helpers"
import type { CanvasRenderService, PlayerData } from "./types"

function CanvasRenderService() {

    let canvas: HTMLCanvasElement
    let ctx: CanvasRenderingContext2D

    let COLOUR = 'blue'
    let SIZE = 10

    async function init() {
        canvas = assignEle<HTMLCanvasElement>("gameCanvas")
        ctx = initCtx(canvas)
    }

    function render(response: PlayerData[]) {
        ctx.clearRect(0, 0, canvas.width, canvas.height)
        ctx.fillStyle = COLOUR
        for (const player of response) {

            if (player.player == "powerup") {
                ctx.fillStyle = 'red'
            } else {
                ctx.fillStyle = COLOUR
            }

            for (const pos of player.chain) {
                ctx.fillRect(pos.x * SIZE, pos.y * SIZE, SIZE, SIZE)
            }
        }
    }
    
    return { init, render }

}

function GameController(canvasRenderService: CanvasRenderService) {

    let ws: WebSocket

    function init({WS_HOST}: {WS_HOST: string}) {

        ws = new WebSocket(WS_HOST)

        ws.onopen = () => {
            ws.send(JSON.stringify({action: "join", username: "chris" }))
        }

        ws.onmessage = (event) => {
            const response: PlayerData[] = JSON.parse(event.data)
            canvasRenderService.render(response)
        }

        document.addEventListener("keydown", (e) => {
            ws.send(JSON.stringify({action: "move", direction: e.key}))
        })
    }

    return { init }
}

async function init() {

    const config = await loadConfig()

    const canvasRenderService = CanvasRenderService()
    canvasRenderService.init()

    const gameController = GameController(canvasRenderService)
    gameController.init({WS_HOST: config.WS_HOST})

}


onload = init