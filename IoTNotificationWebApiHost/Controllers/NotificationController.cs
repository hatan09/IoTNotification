using Microsoft.AspNetCore.Mvc;
using SignalRServices;

namespace IoTNotificationWebApiHost;

[Route("api/[controller]")]
[ApiController]
public class NotificationController(SignalRClientService signalRService) : ControllerBase
{
    #region [ Privaate Fields - Services ]
    private readonly SignalRClientService _signalRService = signalRService ?? throw new ArgumentNullException(nameof(signalRService));

    #endregion
    #region [ CTor ]
    #endregion

    #region [ Methods - Requests ]
    [HttpPost]
    public async Task<IActionResult> SendMessage([FromBody] string message) {
        try {
            //await _signalRService.SendMessageAsync(message);
            return Ok(new { Status = "Message sent", message });
        }
        catch (Exception ex) {
            return StatusCode(500, new { error = ex.Message });
        }
    }

    [HttpGet]
    public async Task<IActionResult> GetMessage() {
        try {
            //await _signalRService.SendMessageAsync(message);
            return Ok(new { Status = "Message sent" });
        }
        catch (Exception ex) {
            return StatusCode(500, new { error = ex.Message });
        }
    }
    #endregion
}
