using Microsoft.Extensions.DependencyInjection;

namespace SignalRServices;

public static class SignalRServiceRegistration
{
    public static void AddSignalRServices(this IServiceCollection services) {
        services.AddSingleton<SignalRClientService>();
    }
}
