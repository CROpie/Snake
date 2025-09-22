import { assignEle, initCtx, loadConfig } from "./helpers/helpers"
import type { CanvasRenderService, SnakeResponse, PlayerData, Position } from "./types"

function CanvasRenderService() {

    let canvas: HTMLCanvasElement
    let ctx: CanvasRenderingContext2D

    let WALL_COLOUR = 'green'
    let COLOUR = 'blue'

    const RATIO = 3

    const SIZE = 10 * RATIO

    const OFFSET = 20

    const gridSprite = new Image()
    const wallSprite = new Image()
    const powerupSprite = new Image()
    const snakeSprite = new Image()

    let walls: Position[] = []

    async function init({IMAGE_PATH}: {IMAGE_PATH: string}) {
        canvas = assignEle<HTMLCanvasElement>("gameCanvas")
        ctx = initCtx(canvas)

        gridSprite.src = `${IMAGE_PATH}/images/Grid.png`
        wallSprite.src = `${IMAGE_PATH}/images/Walls.png`
        powerupSprite.src = `${IMAGE_PATH}/images/Powerups.png`
        snakeSprite.src = `${IMAGE_PATH}/images/Snake.png`
    }

    function setWalls(WALLS: Position[]) {
        walls = WALLS
    }

    function render(response: PlayerData[]) {
        ctx.clearRect(0, 0, canvas.width, canvas.height)


        /* GRID */
        const LENGTH = 300

        ctx.drawImage(
            gridSprite, // sprite sheet
            0, 0, // source X, Y (from top-left corner)
            1000, 1000, // width and height to copy from image
            0, 0, // destination X, Y (from top-left corner)
            LENGTH, LENGTH // width and height to draw on canvas
        )

        ctx.drawImage(
            gridSprite, // sprite sheet
            0, 0, // source X, Y (from top-left corner)
            1000, 1000, // width and height to copy from image
            LENGTH, 0, // destination X, Y (from top-left corner)
            LENGTH, LENGTH // width and height to draw on canvas
        )

        ctx.drawImage(
            gridSprite, // sprite sheet
            0, 0, // source X, Y (from top-left corner)
            1000, 1000, // width and height to copy from image
            0, LENGTH, // destination X, Y (from top-left corner)
            LENGTH, LENGTH // width and height to draw on canvas
        )

        ctx.drawImage(
            gridSprite, // sprite sheet
            0, 0, // source X, Y (from top-left corner)
            1000, 1000, // width and height to copy from image
            LENGTH, LENGTH, // destination X, Y (from top-left corner)
            LENGTH, LENGTH // width and height to draw on canvas
        )

        /* WALL */
        ctx.fillStyle = WALL_COLOUR
        for (const wall of walls) {
            // ctx.fillRect(wall.x * SIZE, wall.y * SIZE, SIZE, SIZE)
            ctx.drawImage(
                wallSprite, // sprite sheet
                getRandom(10), 0, // source X, Y (from top-left corner)
                100, 100, // width and height to copy from image
                wall.x * SIZE, wall.y * SIZE, // destination X, Y (from top-left corner)
                SIZE, SIZE // width and height to draw on canvas
            )
        }

        /* POWERUP */
        ctx.fillStyle = COLOUR
        for (const player of response) {

            if (player.player == "powerup") {
                ctx.fillStyle = 'red'
                renderPowerup(player)
            } else {
                renderSnake(player, player.colour)
            }
        }


    }

    function renderPowerup(player: PlayerData) {
        for (const pos of player.chain) {
            // ctx.fillRect(pos.x * SIZE, pos.y * SIZE, SIZE, SIZE)
            ctx.drawImage(
                powerupSprite, // sprite sheet
                getRandom(10) * 100 - OFFSET, 0 - OFFSET, // source X, Y (from top-left corner)
                100, 100, // width and height to copy from image
                pos.x * SIZE, pos.y * SIZE, // destination X, Y (from top-left corner)
                SIZE, SIZE // width and height to draw on canvas
            )
        }
    }

    enum Ternary {
        gt = 1,
        lt = -1,
        eq = 0,
      }

    function ternary(a: number, b: number): Ternary {
        if (a < b) return Ternary.lt
        if (a > b) return Ternary.gt
        return Ternary.eq
    }

    type ShapeData = {
        isPrev: boolean
        isNext: boolean
        prevY?: Ternary
        prevX?: Ternary
        nextY?: Ternary
        nextX?: Ternary
    }

    function calcShapeData(cur: Position, prev?: Position, next?: Position): ShapeData {
        let shapeData: ShapeData = {
            isPrev: prev == undefined ? false : true,
            isNext: next == undefined ? false : true
        }

        if (prev) {
            shapeData.prevY = ternary(cur.y, prev.y)
            shapeData.prevX = ternary(cur.x, prev.x)
        }

        if (next) {
            shapeData.nextY = ternary(cur.y, next.y)
            shapeData.nextX = ternary(cur.x, next.x)
        }
        return shapeData
    }

    function handleSnakeEnd(shapeData: ShapeData): Position {
        if (shapeData?.prevY == Ternary.eq || shapeData?.nextY == Ternary.eq) {
            // horizontal ** Double-check logic here, vertical was fine, horizontal was backwards
            if (shapeData?.prevX == Ternary.lt || shapeData?.nextX == Ternary.lt) {
                // 56
                return offsetSnakePos(0, 200)
            } else {
                // 45
                return offsetSnakePos(100, 200)
            }
        } else {
            // vertical
            if (shapeData?.prevY == Ternary.lt || shapeData?.nextY == Ternary.lt) {
                // 25
                return offsetSnakePos(400, 200)
            } else {
                // 58
                return offsetSnakePos(400, 300)
            }
        }
    }

    function handleSegment(shapeData: ShapeData): Position {
        if (shapeData.prevY == Ternary.eq && shapeData.nextY == Ternary.eq) {
            // 456
            return offsetSnakePos(100, 100)
        }

        if (shapeData.prevX == Ternary.eq && shapeData.nextX == Ternary.eq) {
            // 258
            return offsetSnakePos(800, 300)
        }

        if ((shapeData.prevX == Ternary.lt && shapeData.nextY == Ternary.lt) || (shapeData.prevY == Ternary.lt && shapeData.nextX == Ternary.lt)) {
            // 245
            return offsetSnakePos(0, 400) // swapped with 568
        }

        if ((shapeData.prevX == Ternary.gt && shapeData.nextY == Ternary.gt) || (shapeData.prevY == Ternary.gt && shapeData.nextX == Ternary.gt)) {
            // 568
            return offsetSnakePos(500, 500) // swapped with 245
        }

        if ((shapeData.prevX == Ternary.gt && shapeData.nextY == Ternary.lt) || (shapeData.prevY == Ternary.lt && shapeData.nextX == Ternary.gt)) {
            // 256
            return offsetSnakePos(300, 400) // swapped with 458
        }

        // 458
        return offsetSnakePos(600, 500) // swapped with 256
    }

    function findSnakeSegmentSprite(cur: Position, prev?: Position, next?: Position): Position {
        const shapeData = calcShapeData(cur, prev, next)

        if (!shapeData.isPrev && !shapeData.isNext) {
            return offsetSnakePos(0, 0)
        }

        if (shapeData.isPrev && shapeData.isNext) {
            return handleSegment(shapeData)
        }

        return handleSnakeEnd(shapeData)

    }
    
    function renderSnake(player: PlayerData, colour: number) {

        let snakeColour = 'white' // default

        switch (colour) {
            case 0:
                snakeColour = 'red'
                break
            case 1:
                snakeColour = 'orange'
                break   
            case 2:
                snakeColour = 'yellow'
                break
            case 3:
                snakeColour = 'green'
                break
            case 4:
                snakeColour = 'blue'
                break
            case 5:
                snakeColour = 'purple'
                break
        }

        for (let i = 0; i < player.chain.length; i++) {

            const spritePos = findSnakeSegmentSprite(player.chain[i]!, player.chain[i-1], player.chain[i+1])
            drawSnakeSegment(player.chain[i]!, spritePos, snakeColour)
        }
    }

    function getRandom(range: number) {
        return Math.floor(Math.random() * range)
    }

    function drawSnakeSegment(current: Position, spritePos: Position, snakeColour: string) {

        ctx.drawImage(
            snakeSprite, // sprite sheet
            spritePos.x, spritePos.y, // source X, Y (from top-left corner)
            100, 100, // width and height to copy from image
            current.x * SIZE, current.y * SIZE, // destination X, Y (from top-left corner)
            SIZE, SIZE // width and height to draw on canvas
        )

        // overlay with chosen color
        ctx.fillStyle = snakeColour;
        ctx.globalCompositeOperation = "source-atop";
        ctx.fillRect(current.x * SIZE, current.y * SIZE, SIZE, SIZE);

        // reset blending mode
        ctx.globalCompositeOperation = "source-over";
    }

    function offsetSnakePos(x: number, y: number): Position {
        let offsetSpritePos: Position = { x: x - OFFSET, y: y - OFFSET}
        return offsetSpritePos
    }


    return { init, setWalls, render }

}

function GameController(canvasRenderService: CanvasRenderService) {

    let ws: WebSocket
    let username = ''
    let playerLives: HTMLUListElement

    function init({WS_HOST, userData}: {WS_HOST: string, userData: any}) {

        playerLives = assignEle<HTMLUListElement>("playerLives")
        
        if (!userData) throw new Error("No user data")

        username = JSON.parse(userData).username ?? "unknown"

        ws = new WebSocket(WS_HOST)

        ws.onopen = () => {
            ws.send(JSON.stringify({action: "join", username }))
        }

        ws.onmessage = (event) => {
            const response: SnakeResponse = JSON.parse(event.data)

            // console.log(response)

            if (response.type === "walls") {
                canvasRenderService.setWalls(response.walls)
                return
            }

            if (response.type === "gameState") {
                canvasRenderService.render(response.gameState)
                listPlayerLives(response.gameState)
                return
            }            
        }

        document.addEventListener("keydown", (e) => {
            ws.send(JSON.stringify({action: "move", direction: e.key}))
        })
    }

    function listPlayerLives(gameState: PlayerData[]) {
        playerLives.innerHTML = ""
        for (const player of gameState) {
            if (player.player == "powerup") continue
            const li = document.createElement('li')
            li.textContent = `${player.player}: ${player.lives}`
            playerLives.appendChild(li)
        }
    }

    return { init }
}

function AuthService() {

    let lobbyUrl = ''
    let authBaseUrl = ''
    let userData = ''

    async function validateToken(): Promise<boolean> {
        try {

            const response = await fetch(`${authBaseUrl}/api/authenticate`, {
                    method: "GET",
                    credentials: 'include'
                })
    
            let json;

            // just handing the situation where data might be empty or not json
            try {
                json = await response.json()
            } catch {
                json = null
            }
    
            if (!response.ok) {
                console.error(json?.error ?? "something went wrong...")
                return false
            }
    
            if (!json?.data) {
                console.error("Response did not contain data object")
                return false
            }

            userData = JSON.stringify(json.data)
    
            return true
    
        } catch (error) {
            console.error("Network error: request failed")
            return false
        }
    }

    function init({LOBBY_URL, AUTH_BASE_URL}: {LOBBY_URL: string, AUTH_BASE_URL: string}) {
        lobbyUrl = LOBBY_URL
        authBaseUrl = AUTH_BASE_URL
    }

    function sendToReturnUrl() {
        window.location.href = lobbyUrl
    }

    function getUserData() {
        return userData
    }



    return { init, sendToReturnUrl, validateToken, getUserData }
}

async function init() {

    const config = await loadConfig()

    const authService = AuthService()

    authService.init({ LOBBY_URL: config.LOBBY_URL, AUTH_BASE_URL: config.AUTH_BASE_URL })

    if (!await authService.validateToken()){
        authService.sendToReturnUrl()
        return
    }

    const canvasRenderService = CanvasRenderService()
    canvasRenderService.init({IMAGE_PATH: config.IMAGE_PATH})

    const gameController = GameController(canvasRenderService)
    gameController.init({WS_HOST: config.WS_HOST, userData: authService.getUserData()})

}


onload = init