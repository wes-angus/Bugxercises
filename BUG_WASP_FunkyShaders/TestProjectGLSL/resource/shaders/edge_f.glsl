/*
	Edge fragment shader

	Detect edges and blend with the input image.
*/


#version 120


// 4 uniforms
uniform sampler2D inputColour;
uniform sampler2D inputNormal;
uniform sampler2D inputDepth;
uniform vec2 pixelSize;


// return 0 if edge, 1 if not edge
float Sobel_Hori(sampler2D image, vec2 texcoord)
{
	vec2 ps = pixelSize;
	vec2 offset[6] = vec2[](vec2(-ps.s, -ps.t), vec2(ps.s, -ps.t),
							vec2(-ps.s, 0.0), vec2(ps.s, 0.0),
							vec2(-ps.s, ps.t), vec2(ps.s, ps.t));

	vec3 sum = vec3(0.0);
	sum +=     -texture2D(image, offset[0]+texcoord).rgb;
	sum +=	    texture2D(image, offset[1]+texcoord).rgb;
	sum += -2.0*texture2D(image, offset[2]+texcoord).rgb;
	sum +=  2.0*texture2D(image, offset[3]+texcoord).rgb;
	sum +=     -texture2D(image, offset[4]+texcoord).rgb;
	sum +=      texture2D(image, offset[5]+texcoord).rgb;

	float lenSq = dot(sum, sum);
	return (lenSq < 1.0 ? 1.0 : 0.0);
}


float Sobel_Vert(sampler2D image, vec2 texcoord)
{
	vec2 ps = pixelSize;
	vec2 offset[6] = vec2[](vec2(-ps.s, -ps.t), vec2(0.0, -ps.t), vec2(ps.s, -ps.t),
							vec2(-ps.s, ps.t), vec2(0.0, ps.t), vec2(ps.s, ps.t));

	vec3 sum = vec3(0.0);
	sum +=     -texture2D(image, offset[0]+texcoord).rgb;
	sum += -2.0*texture2D(image, offset[1]+texcoord).rgb;
	sum +=     -texture2D(image, offset[2]+texcoord).rgb;
	sum +=      texture2D(image, offset[3]+texcoord).rgb;
	sum +=  2.0*texture2D(image, offset[4]+texcoord).rgb;
	sum +=      texture2D(image, offset[5]+texcoord).rgb;

	float lenSq = dot(sum, sum);
	return (lenSq < 1.0 ? 1.0 : 0.0);
}


void main()
{	
	vec2 texcoord = gl_TexCoord[0].st;
	
	vec3 c = texture2D(inputColour, texcoord).rgb;

	float normalSobelH = Sobel_Hori(inputNormal, texcoord);
	float normalSobelV = Sobel_Vert(inputNormal, texcoord);
	float depthSobelH = Sobel_Hori(inputDepth, texcoord);
	float depthSobelV = Sobel_Vert(inputDepth, texcoord);
	float colourSobelH = Sobel_Hori(inputColour, texcoord);
	float colourSobelV = Sobel_Vert(inputColour, texcoord);

	float finalEdge = normalSobelH * normalSobelV * depthSobelH * depthSobelV * colourSobelH * colourSobelV;

	gl_FragColor.rgb = finalEdge * c;
}