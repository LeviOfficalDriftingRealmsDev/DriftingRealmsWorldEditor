CollisionOutline CreateOutlineFromPoints(std::vector<Vec2f> chain_points){
    CollisionOutline outline = CollisionOutline();

    //! push back contour, when we have multiple ones this will have to change
    (&outline.get_contours())->push_back(Contour());

    //! get a POINTER to the contour points
    auto contour_points = &outline.get_contours()[0].get_points();

    for(auto &chain_point : chain_points){
        contour_points->push_back(Pointf(chain_point));
    }
    outline.calculate_smallest_enclosing_discs();
    outline.calculate_sub_circles();
    outline.calculate_radius();
    return outline;
};
