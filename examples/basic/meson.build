glslc = find_program('glslc')

shaders = [
    'basic.vert',
    'basic.frag',
]

compiled_shaders = []
foreach shader : shaders
    compiled_shaders += custom_target(
        shader + '.spv',
        input  : 'shaders' / shader,
        output : shader + '.spv',
        command : [glslc, '@INPUT@', '-o', '@OUTPUT@'],
        build_by_default : true,
    )
endforeach

sources = files(
    'main.cpp',
)

glfw_dep = dependency('glfw3', required : true)

executable('basic', sources, compiled_shaders, dependencies : [framework_dep, glfw_dep, vma_dep])
