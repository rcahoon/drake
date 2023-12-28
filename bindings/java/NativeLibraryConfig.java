import org.bytedeco.javacpp.*;
import org.bytedeco.javacpp.annotation.*;
import org.bytedeco.javacpp.tools.*;

@Properties(
    value = @Platform(
        includepath = {"/home/parallels/drake"},
        //preloadpath = {"/path/to/deps/"},
        linkpath = {"/home/parallels/drake/bazel-bin/tools/install/libdrake"},
        include = {
            "systems/framework/diagram.h",
            "systems/framework/leaf_system.h",
            "systems/framework/system.h",
            "systems/framework/system_html.h",
            "systems/framework/system_scalar_converter.h",
            "systems/framework/system_visitor.h",
            "systems/framework/vector_system.h",
            "systems/framework/witness_function.h",
	},
        //preload = {"DependentLib"},
        link = {"drake"}
    ),
    target = "NativeLibrary"
)
public class NativeLibraryConfig implements InfoMapper {
    public void map(InfoMap infoMap) {
    }
}
