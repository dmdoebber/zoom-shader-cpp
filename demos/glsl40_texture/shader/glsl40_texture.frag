#version 420

uniform vec2  tiros[10];
uniform float xMouse, yMouse;
uniform float zoom, raioLuneta;
uniform bool  ativo;

in vec2 TexCoord;

layout (binding = 0) uniform sampler2D Texture1;


vec4 colisaoTiro(vec2 tex) //FUN��O PARA VERIFICAR COLIS�O ENTRE AS POSI��ES DE TIROS E O PIXEL DA TEXTURA
{
	for(int i = 0; i < tiros.length; i++)
		if (sqrt(pow(tex.x - tiros[i].x, 2) + pow(tex.y - tiros[i].y, 2)) < 0.001)
			return vec4(1.0f);
	
	return texture(Texture1, tex);
}

void main() 
{
	vec2 mouse  = vec2(xMouse, yMouse);
	vec2 offset = TexCoord - mouse;
	float dist  = sqrt(pow(offset.x, 2) + pow(offset.y, 2));	

	if(ativo) //VERIFICA SE A MIRA ESTA ATIVA
	{
		if(dist < raioLuneta) //VERIFICA SE O PIXEL EST� DENTRO DO RAIO DA LUNETA
		{
			if(dist < raioLuneta - 0.003) // IMPRIME NA TELA O CIRCULO E A CRUZ DA MIRA
			{ 
				if (TexCoord.x > mouse.x - 0.001 && TexCoord.x < mouse.x + 0.001 || TexCoord.y > mouse.y - 0.001 && TexCoord.y < mouse.y + 0.001)	
				{
					gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0f); 
				}
				else // CALCULA O ZOOM USANDO COORDENADAS POLARES DESCOBRE O ANGULO DO PIXEL E MULTIPLICA O RAIO PELO ZOOM
				{
					float ang    = atan(offset.y, offset.x);
					float raio   = dist * zoom;
					vec2 tex     = vec2(xMouse + cos(ang) * raio, yMouse + sin(ang) * raio);
					gl_FragColor = colisaoTiro(tex);
				}
			}
		}
		else //SE ESTIVER FORA DA MIRA, DEIXA A IMAGEM EMBA�ADA
		{	
			gl_FragColor = colisaoTiro(TexCoord);
			if(!(gl_FragColor == vec4(1.0f))) //VERIFICA SE O PIXEL N�O ESTA EM UMA COORDENADA DE TIO
			{
				vec3 color = vec3(0.0f);
				float raio, ang, div = 0.0f;
				for(raio = 0.01; raio < 0.03; raio += 0.01)
					for(ang = 0.0; ang < 6.28; ang += 0.1, div++)
						color += texture(Texture1, vec2(TexCoord.x +  raio * cos(ang), TexCoord.y + raio * sin(ang))).rgb;
			
				gl_FragColor = vec4(color /= div, 1.0f);
			}			
		}	
	}
	else //SE N�O ESTIVER COM A MIRA ATIVA S� IMPRIME A TEXTURA NORMALMENTE, POR�M MANTENDO AS POSI��ES DE TIROS
	{
		gl_FragColor = colisaoTiro(TexCoord);
	}	
}

