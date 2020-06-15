#version 410 core
layout (location = 0) in vec3 aPos;
uniform vec2 scale;
uniform ivec2 screenDimensions;
uniform vec3 translate;
uniform float rotateDegrees;


/** Basic vertex shader that handles rotation, scaling, translation, and
    screen scaling. As this is a 2d renderer at this point, the z axis
    is not mutated at all. It's used only for ordering overlaps.
*/
void main()
{
    float radians = radians(rotateDegrees);

    vec2 rotatedPos = vec2(
        aPos.x * cos(radians) - (aPos.y * sin(radians)),
        aPos.y * cos(radians) + aPos.x * sin(radians)
    );
    vec2 scaledPos = vec2(
        (rotatedPos.x * scale.x),
        (rotatedPos.y * scale.y)
    );
    vec3 translatedPos = vec3(
        scaledPos.x + translate.x,
        scaledPos.y + translate.y,
        aPos.z + translate.z
    );
    vec2 screenAdjustedPos = vec2(
        (translatedPos.x - (screenDimensions.x / 2)) / (screenDimensions.x / 2),
        (translatedPos.y - (screenDimensions.y / 2)) / (screenDimensions.y / 2)
        // don't need to screen adjust the z axis. It's just for overlap order.
    );
    gl_Position = vec4(screenAdjustedPos.x, screenAdjustedPos.y, translatedPos.z, 1.0);
}