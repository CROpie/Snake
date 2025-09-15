function init() {
    const canvas = document.getElementById("gameCanvas") as HTMLCanvasElement
    if (!canvas) throw new Error("Canvas not found")

    const ctx = canvas.getContext("2d")

    if (!ctx) throw new Error("No ctx")

    const ws = new WebSocket("ws://127.0.0.1:9023")

    ws.onopen = () => {
        ws.send(JSON.stringify({action: "join" }))
    }

    ws.onmessage = (event) => {
        const response = JSON.parse(event.data)
        render(canvas, ctx, response)
    }

    document.addEventListener("keydown", (e) => {
        console.log(e.key)
        ws.send(JSON.stringify({action: "move", direction: e.key}))
    })
}

function render(canvas: HTMLCanvasElement, ctx: CanvasRenderingContext2D, response: {x: number, y: number}) {
    ctx.clearRect(0, 0, canvas.width, canvas.height)
    ctx.fillStyle = 'blue'
    ctx.fillRect(response.x * 10, response.y * 10, 10, 10)
}

onload = init