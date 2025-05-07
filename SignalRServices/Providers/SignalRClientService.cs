using Microsoft.AspNetCore.SignalR.Client;
using Microsoft.Extensions.Logging;

namespace SignalRServices;
public class SignalRClientService
{
    private readonly HubConnection _connection;
    private readonly ILogger<SignalRClientService> _logger;
    private readonly SignalRClientSettings _settings;

    public SignalRClientService(ILogger<SignalRClientService> logger, SignalRClientSettings settings) {
        _logger = logger;
        _settings = settings;

        _connection = new HubConnectionBuilder()
            .WithUrl(settings.HubUrl)
            .WithAutomaticReconnect()
            .Build();

        _connection.Reconnecting += error => {
            _logger.LogWarning("Reconnecting due to: {Error}", error?.Message);
            return Task.CompletedTask;
        };

        _connection.Reconnected += connectionId => {
            _logger.LogInformation("Reconnected with connection ID: {ConnectionId}", connectionId);
            return Task.CompletedTask;
        };

        _connection.Closed += error => {
            _logger.LogError("Connection closed due to: {Error}", error?.Message);
            return Task.CompletedTask;
        };
    }

    public async Task EnsureConnectedAsync() {
        if (_connection.State == HubConnectionState.Disconnected) {
            await _connection.StartAsync();
        }
    }

    public async Task SendMessageAsync(string message) {
        await EnsureConnectedAsync();
        await _connection.SendAsync("ReceiveMessage", message);
    }
}
