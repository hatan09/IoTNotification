using System.Reflection;
using Microsoft.OpenApi.Models;

namespace IoTNotificationWebApiHost;

public static class RegisterSwaggerServices
{
    #region [ Public Methods - IServiceCollection ]
    public static void AddSwaggerDefaults(this IServiceCollection services, string serviceName, int version = 1) {

        var versionName = $"v{version}";

        services.AddSwaggerGen(x => {

            // Schema
            x.CustomSchemaIds(type => type.ToString());

            // Documentation
            x.SwaggerDoc(versionName, new OpenApiInfo() {
                Title = $"{serviceName} WebApi",
                Version = versionName,
                Description = $"This web api gives access to the {serviceName} services."
            });

            var xmlDocumentationFileName = $"{Assembly.GetExecutingAssembly().GetName().Name}.xml";
            var xmlDocumentationFilePath = Path.Combine(AppContext.BaseDirectory, xmlDocumentationFileName);

            if (File.Exists(xmlDocumentationFilePath)) {
                x.IncludeXmlComments(xmlDocumentationFilePath);
            }

            // Security
            x.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme() {
                Name = "Authorization",
                Type = SecuritySchemeType.ApiKey,
                Scheme = "Bearer",
                BearerFormat = "JWT",
                In = ParameterLocation.Header,
                Description = "JWT Authorization header using the Bearer scheme. \r\n\r\n Enter 'Bearer' [space] and then your token in the text input below.\r\n\r\nExample: \"Bearer F1asA#-Asxzq4@\"",
            });
            x.AddSecurityRequirement(new OpenApiSecurityRequirement {
                {
                    new OpenApiSecurityScheme {
                        Reference = new OpenApiReference {
                            Type = ReferenceType.SecurityScheme,
                                Id = "Bearer"
                        }
                    },
                    Array.Empty<string>()
                }
            });
        });
    }
    #endregion
}
