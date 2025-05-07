using Microsoft.AspNetCore.SignalR;

namespace IoTNotification;

public sealed class IoTHub : Hub
{
    public override async Task OnConnectedAsync() {
        var httpContext = Context.GetHttpContext();
        string userId = httpContext?.Request.Query["userId"]; // Get userId from query

        await Clients.All.SendAsync("ReceiveMessage", $"{userId} ({Context.ConnectionId}) connected");
        await base.OnConnectedAsync();
    }

    public override async Task OnDisconnectedAsync(Exception? exception) {
        await Clients.All.SendAsync("ReceiveMessage", $"{Context.ConnectionId} disconnected");
        await base.OnDisconnectedAsync(exception);
    }

    public async Task SendMessage(string message) {
        await Clients.All.SendAsync("ReceiveMessage", $"{Context.ConnectionId}: {message}");
    }

    public async Task StartDrag(string taskId) {
        // Notify all clients except the sender
        await Clients.Others.SendAsync("TaskDragStart", taskId, false);
    }
    public async Task TaskDropComplete(string taskId, string groupName, int orderIndex) {
        await Clients.All.SendAsync("TaskDropComplete", taskId, true, groupName, orderIndex);
    }
}