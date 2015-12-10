
#include <mbgl/style/types.hpp>


namespace mbgl {

	::mbgl::EnumValue<StyleLayerType> StyleLayerType_names[] = {
		{ StyleLayerType::Unknown, "unknown" },
		{ StyleLayerType::Fill, "fill" },
		{ StyleLayerType::Line, "line" },
		{ StyleLayerType::Symbol, "symbol" },
		{ StyleLayerType::Raster, "raster" },
		{ StyleLayerType::Background, "background" },
		{ StyleLayerType(-1), "unknown" },
	};

	
	inline std::ostream& operator<<(std::ostream& os, StyleLayerType t) { return os << StyleLayerTypeClass(t).str(); }

	::mbgl::EnumValue<SourceType> SourceType_names[] = {
		{ SourceType::Vector, "vector" },
		{ SourceType::Raster, "raster" },
		{ SourceType::GeoJSON, "geojson" },
		{ SourceType::Video, "video" },
		{ SourceType::Annotations, "annotations" },
	};

	
	inline std::ostream& operator<<(std::ostream& os, SourceType t) { return os << SourceTypeClass(t).str(); }


	::mbgl::EnumValue<VisibilityType> VisibilityType_names[] = {
		{ VisibilityType::Visible, "visible" },
		{ VisibilityType::None, "none" },
	};

	
		inline std::ostream& operator<<(std::ostream& os, VisibilityType t) { return os << VisibilityTypeClass(t).str(); }

	::mbgl::EnumValue<CapType> CapType_names[] = {
		{ CapType::Round, "round" },
		{ CapType::Butt, "butt" },
		{ CapType::Square, "square" },
	};

	
	inline std::ostream& operator<<(std::ostream& os, CapType t) { return os << CapTypeClass(t).str(); }

	::mbgl::EnumValue<JoinType> JoinType_names[] = {
		{ JoinType::Miter, "miter" },
		{ JoinType::Bevel, "bevel" },
		{ JoinType::Round, "round" },
		{ JoinType::FlipBevel, "flipbevel" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, JoinType t) { return os << JoinTypeClass(t).str(); }

	::mbgl::EnumValue<TranslateAnchorType> TranslateAnchorType_names[] = {
		{ TranslateAnchorType::Map, "map" },
		{ TranslateAnchorType::Viewport, "viewport" },
	};

	
		inline std::ostream& operator<<(std::ostream& os, TranslateAnchorType t) { return os << TranslateAnchorTypeClass(t).str(); }
	
	::mbgl::EnumValue<RotateAnchorType> RotateAnchorType_names[] = {
		{ RotateAnchorType::Map, "map" },
		{ RotateAnchorType::Viewport, "viewport" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, RotateAnchorType t) { return os << RotateAnchorTypeClass(t).str(); }

	::mbgl::EnumValue<PlacementType> PlacementType_names[] = {
		{ PlacementType::Point, "point" },
		{ PlacementType::Line, "line" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, PlacementType t) { return os << PlacementTypeClass(t).str(); }

	::mbgl::EnumValue<RotationAlignmentType> RotationAlignmentType_names[] = {
		{ RotationAlignmentType::Map, "map" },
		{ RotationAlignmentType::Viewport, "viewport" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, RotationAlignmentType t) { return os << RotationAlignmentTypeClass(t).str(); }

	::mbgl::EnumValue<TextJustifyType> TextJustifyType_names[] = {
		{ TextJustifyType::Center, "center" },
		{ TextJustifyType::Left, "left" },
		{ TextJustifyType::Right, "right" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, TextJustifyType t) { return os << TextJustifyTypeClass(t).str(); }
	::mbgl::EnumValue<TextAnchorType> TextAnchorType_names[] = {
		{ TextAnchorType::Center, "center" },
		{ TextAnchorType::Left, "left" },
		{ TextAnchorType::Right, "right" },
		{ TextAnchorType::Top, "top" },
		{ TextAnchorType::Bottom, "bottom" },
		{ TextAnchorType::TopLeft, "top-left" },
		{ TextAnchorType::TopRight, "top-right" },
		{ TextAnchorType::BottomLeft, "bottom-left" },
		{ TextAnchorType::BottomRight, "bottom-right" }
	}; 
	
		inline std::ostream& operator<<(std::ostream& os, TextAnchorType t) { return os << TextAnchorTypeClass(t).str(); }

	::mbgl::EnumValue<TextTransformType> TextTransformType_names[] = {
		{ TextTransformType::None, "none" },
		{ TextTransformType::Uppercase, "uppercase" },
		{ TextTransformType::Lowercase, "lowercase" },
	};
	
		inline std::ostream& operator<<(std::ostream& os, TextTransformType t) { return os << TextTransformTypeClass(t).str(); }

}