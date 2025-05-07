using IoTNotificationWebApiHost;

var builder = WebApplication.CreateBuilder(args);

// Configuration
var secretsSectionName = builder.Configuration.GetValue<string>("UseSecretsSection");

// Key Vault
//if (string.IsNullOrEmpty(secretsSectionName)) {
//    var options = new AzureKeyVaultConfigurationOptions() {
//        ReloadInterval = TimeSpan.FromMinutes(10),
//    };

//    builder.Configuration.AddAzureKeyVault(new Uri(builder.Configuration["KEY_VAULT_URI"]),
//                                           new DefaultAzureCredential(),
//                                           options);

//}
//else {
//    var secretSection = builder.Configuration.GetSection(secretsSectionName);
//    var sp_tenant = secretSection.GetValue<string>("sp_tenant");
//    var sp_client = secretSection.GetValue<string>("sp_client");
//    var sp_secret = secretSection.GetValue<string>("sp_secret");
//    builder.Configuration.AddAzureKeyVault(new Uri(secretSection.GetValue<string>("KEY_VAULT_URI")),
//                                           new ClientSecretCredential(sp_tenant, sp_client, sp_secret));
//}

// Health checks
builder.Services.AddHealthChecks();

// Add services to the container.
builder.Services.RegisterSignalRClient(builder.Configuration);

// Web Api
builder.Services.AddHttpContextAccessor();
builder.Services.AddControllers();
builder.Services.AddEndpointsApiExplorer();

// Swagger
builder.Services.AddSwaggerDefaults("Notification Api", 1);

// Run
var app = builder.Build();

app.MapControllers();
app.UseSwagger();
app.UseSwaggerUI();

app.MapHealthChecks("/healthz");

app.Run();
