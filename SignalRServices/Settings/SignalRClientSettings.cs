namespace SignalRServices;

public class SignalRClientSettings
{
    #region [ CTor ]
    public SignalRClientSettings() {

    }

    public SignalRClientSettings(string hubUrl, string hubName) {
        HubUrl = hubUrl;
        HubName = hubName;
    }
    #endregion

    #region [ Properties ]  
    public string HubUrl { get; internal set; }

    public string HubName { get; internal set; }
    #endregion
}
