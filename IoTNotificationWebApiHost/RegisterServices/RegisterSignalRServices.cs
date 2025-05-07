using SignalRServices;

namespace IoTNotificationWebApiHost;

internal static class RegisterSignalRServices
{
    #region [ Methods ]
    /// <summary>
    /// Registers the SignalR client services.
    /// </summary>
    /// <param name="services"></param>
    /// <param name="configuration"></param>
    internal static void RegisterSignalRClient(this IServiceCollection services, IConfiguration configuration) {
        EnsureSettings(services, configuration);

        // SignalRClient
        services.AddSignalRServices();
    }
    #endregion

    #region [ Methods ]
    /// <summary>
    /// Ensures that the SignalRClient settings are set in the configuration.
    /// </summary>
    /// <param name="services"></param>
    /// <param name="configuration"></param>
    /// <exception cref="ArgumentException"></exception>
    private static void EnsureSettings(this IServiceCollection services, IConfiguration configuration) {
        var settings = new SignalRClientSettings();

        configuration.GetSection("SignalRClient").Bind(settings);

        if (string.IsNullOrEmpty(settings.HubUrl)) {
            throw new ArgumentException("SignalRClient: required field HubUrl is not set in the configuration.");
        }

        services.AddSingleton(settings);
    }
    #endregion
}
