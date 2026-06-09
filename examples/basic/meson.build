glslc = find_program('glslc')

shaders = [
    'basic.vert',
    'basic.frag',
]

compiled_shaders = []
foreach shader : shaders
    compiled_shaders += custom_target(
        shader + '.spv',
        input : 'shaders' / shader,
        output : shader + '.spv',
        command : [glslc, '@INPUT@', '-o', '@OUTPUT@'],
        build_by_default : true,
    )
endforeach

sources = files(
    'main.cpp',
)

glfw_dep = dependency('glfw3', required : true)

basic_warnings = compiler.get_supported_arguments(
    '-Wno-missing-field-initializers'
)

executable('basic', sources, compiled_shaders, cpp_args : basic_warnings, dependencies : [framework_dep, glfw_dep, vma_dep])
